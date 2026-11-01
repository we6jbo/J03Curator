#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDirIterator>
#include <QFileDevice>
#include <QJsonParseError>
#include <QSaveFile>
#include <QStandardPaths>
#include <QUrl>
#include <QTimer>

static const char *PROVENANCE_CODES="TG812186,TG323932,TG796320,TG654173,TG654147,TG564843,TG333041,TG610982,TG148675";
static QTextStream out(stdout), err(stderr);
static QString repo() { return QDir::homePath()+"/.j03-software"; }
static QString dbfile() { return QDir::homePath()+"/.so672jNC/j03curator.db"; }
static QString run(const QString &exe,const QStringList &args,int *rc=nullptr) {
    QProcess p; p.setWorkingDirectory(repo()); p.start(exe,args);
    if(!p.waitForStarted(5000)){if(rc)*rc=127;return "Unable to start "+exe;}
    p.waitForFinished(60000); if(rc)*rc=(p.exitStatus()==QProcess::NormalExit)?p.exitCode():128;
    return QString::fromUtf8(p.readAllStandardOutput()+p.readAllStandardError()).trimmed();
}
static bool execSql(QSqlDatabase &db,const QString &sql) { QSqlQuery q(db);if(!q.exec(sql)){err<<q.lastError().text()<<Qt::endl;return false;}return true; }
static void help(){out<<"J03Curator v0.7 - CLI only | provenance: "<<PROVENANCE_CODES<<"\n"
 "Commands:\n  status\n  init\n  add <project> <path-to-main.cpp-or-file>\n  list\n  set <project> copyright|useful yes\n  review <project> (confirm flags and mark reviewed)\n  plan <project>\n  handoff <project> (copyable AI next-step guidance)\n  manual (display manual location)\n  sync [--apply] (repository-only maintenance; explicit Git push)\n  instructions <project>\n  genealogy\n  export <project> [--apply] (approved documentation only; no push)\n  screenshot <project> [--select-latest] (local-only, no upload)\n  completion start|check|slow|blog <project> (timed AI handoff; no publication)\n"<<Qt::endl;}

// Completion timing is persisted outside /tmp so a T14 crash cannot reset the 16-minute clock.
static QString durableTiming() { return QDir::homePath()+"/.local/state/j03curator/completion-time.json"; }
static QString transientTiming() { return "/tmp/emal/time.json"; }
static bool writeJson(const QString &path,const QJsonObject &obj) {
    if(!QDir().mkpath(QFileInfo(path).absolutePath()))return false;
    QSaveFile f(path);if(!f.open(QIODevice::WriteOnly))return false;
    f.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));return f.commit();
}
static QJsonObject readJson(const QString &path) {
    QFile f(path);if(!f.open(QIODevice::ReadOnly))return {};
    auto d=QJsonDocument::fromJson(f.readAll());return d.isObject()?d.object():QJsonObject{};
}
static QString completionPrompt(const QString &project,const QString &why) {
    return "ChatGPT or Mistral Vibe: The completion phase for "+project+" needs help because "+why+". "
    "Review the work already completed and the user's approvals before proceeding. "
    "Please simplify the remaining steps and guide Jeremiah one pasteable command at a time. "
    "Make a new J03Curator release only if an implementation change is needed; provide a verified ZIP and installer, "
    "and separately assess J03Curator's own release eligibility for GitHub, Flatpak/Flathub, Snap package and Snap Store. "
    "Do not apply CraftBox's documentation-only release decision to J03Curator or other projects. "
    "For each project, export ONLY reviewed, legally redistributable and useful source, assets and AI instructions; "
    "include one specifically approved screenshot (or explicitly report if none can be captured). "
    "Never assume approval to publish or release to a store. Check rights, privacy, code quality, links and actual status. "
    "J03Curator manual: /home/we6jbo/.jchjtzir/manual.md ; "
    "J03Curator source: /home/we6jbo/Projects/J03Curator/ . "
    "Recover completion timer from ~/.local/state/j03curator/completion-time.json if /tmp/emal/time.json was lost. "
    "Session URL is in /home/we6jbo/.ckbrgj/chatgpt-session.txt .";
}
static void offerClipboard(const QString &message) {
    QString clip=QStandardPaths::findExecutable("wl-copy");QStringList args;
    if(clip.isEmpty()){clip=QStandardPaths::findExecutable("xclip");args={"-selection","clipboard"};}
    if(clip.isEmpty()){clip=QStandardPaths::findExecutable("xsel");args={"--clipboard","--input"};}
    if(clip.isEmpty()){out<<"Clipboard utility unavailable. Copy the message above manually.\n";return;}
    QProcess proc;proc.start(clip,args);if(!proc.waitForStarted(2000)){out<<"Clipboard process failed; copy manually.\n";return;}
    proc.write(message.toUtf8());proc.closeWriteChannel();
    if(!proc.waitForFinished(4000) || proc.exitCode()!=0)out<<"Clipboard was not confirmed; copy manually.\n";
    else out<<"AI message copied to clipboard. Paste into your session.\n";
}
static void openSession() {
    QFile f(QDir::homePath()+"/.ckbrgj/chatgpt-session.txt");
    if(!f.open(QIODevice::ReadOnly)){out<<"Session file unavailable; no browser launched.\n";return;}
    QString url=QString::fromUtf8(f.readAll()).trimmed();
    QUrl u(url);
    if(!u.isValid()||u.scheme()!="https"||u.host()!="chatgpt.com"||!u.path().startsWith("/c/")){
        out<<"Session file must contain a single https://chatgpt.com/c/... URL; browser not launched.\n";return;
    }
    auto chrome=QStandardPaths::findExecutable("google-chrome-stable");
    if(chrome.isEmpty()){out<<"google-chrome-stable unavailable; open the saved session URL yourself.\n";return;}
    if(QProcess::startDetached(chrome,{url}))out<<"Opened saved ChatGPT session in Chrome.\n";
    else out<<"Chrome could not be started.\n";
}
static int completionCommand(const QStringList &args) {
    if(args.size()!=4){err<<"Usage: J03Curator completion start|check|slow|blog PROJECT\n";return 2;}
    const QString action=args[2],project=args[3];
    if(project.isEmpty()||project=="."||project==".."||project.contains('/')||project.contains('\\')){err<<"Invalid project name.\n";return 2;}
    if(action!="start"&&action!="check"&&action!="slow"&&action!="blog"){err<<"Unknown completion action.\n";return 2;}
    QJsonObject saved=readJson(durableTiming());QJsonObject tmp=readJson(transientTiming());
    if(action=="start") {
        QJsonObject state=saved;
        // An existing unfinished phase remains the same phase; don't reset the clock.
        if(state.value("project").toString()!=project || state.value("blog_published").toBool(false) ||
            !QDateTime::fromString(state.value("started_at").toString(),Qt::ISODate).isValid()) {
            state={{"project",project},{"started_at",QDateTime::currentDateTime().toString(Qt::ISODate)},
                   {"blog_published",false},{"schema",1}};
        }
        if(!writeJson(durableTiming(),state) || !writeJson(transientTiming(),state)){err<<"Timer save failed. Check directory permissions.\n";return 3;}
        out<<"Completion phase started/restored for "<<project<<" at "<<state.value("started_at").toString()<<".\n";
        out<<"Timer saved to /tmp/emal/time.json and durable checkpoint "<<durableTiming()<<".\n";return 0;
    }
    QJsonObject state=saved;
    if(state.value("project").toString()!=project && tmp.value("project").toString()==project)state=tmp;
    if(state.value("project").toString()!=project){err<<"No timer for this project. Start with J03Curator completion start "<<project<<".\n";return 2;}
    // Restore the original timestamp after /tmp was wiped; don't fabricate a new start time.
    if(tmp!=state && !writeJson(transientTiming(),state))out<<"Warning: unable to restore /tmp timer.\n";
    auto begin=QDateTime::fromString(state.value("started_at").toString(),Qt::ISODate);
    if(!begin.isValid()){err<<"Timer is invalid. Original start time cannot be recovered.\n";return 3;}
    if(action=="blog") {
        out<<"Has the blog actually been published? [Enter] Yes / [cancel] No: "<<Qt::flush;
        QTextStream input(stdin);auto answer=input.readLine();
        if(answer.isNull()||!answer.trimmed().isEmpty()){out<<"Cancelled; timer unchanged.\n";return 0;}
        state.insert("blog_published",true);state.insert("blog_confirmed_at",QDateTime::currentDateTime().toString(Qt::ISODate));
        if(!writeJson(durableTiming(),state)||!writeJson(transientTiming(),state)){err<<"Could not persist blog status.\n";return 3;}
        out<<"Blog marked completed for this timer; no website changes made.\n";return 0;
    }
    auto elapsed=qMax(qint64(0),begin.secsTo(QDateTime::currentDateTime()));
    bool overdue=elapsed>16*60 && !state.value("blog_published").toBool(false);
    out<<"Completion project: "<<project<<" | started: "<<state.value("started_at").toString()
       <<" | elapsed minutes: "<<(elapsed/60)<<" | blog recorded: "<<(state.value("blog_published").toBool()?"yes":"no")<<".\n";
    if(action=="check"&&!overdue){out<<"No escalation needed yet. For immediate help: J03Curator completion slow "<<project<<"\n";return 0;}
    if(action=="slow"||overdue) {
        QString message=completionPrompt(project,action=="slow"?"Jeremiah says this is taking too long":"more than 16 minutes passed without a recorded blog");
        out<<"\n--- COPYABLE AI HANDOFF ---\n"<<message<<"\n--- END HANDOFF ---\n";
        offerClipboard(message);openSession();
    }
    return 0;
}

int main(int argc,char **argv){
 QCoreApplication app(argc,argv);
 // First invocation in this /tmp lifecycle: tell AI assistants where the manual is.
 // Print before creating the marker, exactly as requested. No marker means retry next time.
 const QString startupMarker = "/tmp/wzfh/started.touch";
 if (!QFileInfo::exists(startupMarker)) {
     out << "Chatgpt or Mistral vibe, please read /home/we6jbo/.jchjtzir/manual.md because it has all the commands" << Qt::endl;
     if (QDir().mkpath("/tmp/wzfh")) {
         QFile marker(startupMarker);
         if (!marker.open(QIODevice::WriteOnly))
             err << "Warning: could not create " << startupMarker << Qt::endl;
         else marker.close();
     } else {
         err << "Warning: could not create /tmp/wzfh" << Qt::endl;
     }
 }
 auto a=app.arguments(); if(a.size()<2){help();return 0;}
 const QString cmd=a[1];
 if(cmd=="completion")return completionCommand(a);
 if(cmd=="manual") {out<<"Manual: "<<QDir::homePath()<<"/.jchjtzir/manual.md\n";return 0;}
 if(cmd=="genealogy") {out<<"Provenance codes: "<<PROVENANCE_CODES<<"\nFred Ernest Hillman: TG796320\nElena M Johannsen: TG654173\n";return 0;}
 if(cmd=="status") {out<<"Repository: "<<repo()<<" ("<<(QDir(repo()).exists()?"exists":"missing")<<")\nDatabase: "<<dbfile()<<" ("<<(QFileInfo::exists(dbfile())?"exists":"not initialized")<<")\n";
 out<<"Git tree: "<<run("git",{"rev-parse","--show-toplevel"})<<"\nGit remote: "<<run("git",{"remote","get-url","origin"})<<"\nMarker local: "<<(QFileInfo::exists(repo()+"/28253.txt")?"yes":"no")<<"\n";return 0;}
 if(cmd=="sync") {
  const bool apply=a.contains("--apply");
  if(!QDir(repo()).exists()){err<<"Repository directory missing.\n";return 2;}
  QString remote=run("git",{"remote","get-url","origin"});
  if(remote!="https://github.com/we6jbo/j03-software.git" && remote!="git@github.com:we6jbo/j03-software.git") {err<<"Unexpected remote; refusing sync: "<<remote<<Qt::endl;return 3;}
  QString mark=repo()+"/28253.txt";
  out<<"Git status:\n"<<run("git",{"status","--short"})<<"\n";
  out<<"Local marker "<<(QFileInfo::exists(mark)?"present":"absent")<<". GitHub marker check requires authenticated network verification.\n";
  if(!apply){out<<"Dry run only. Use sync --apply after reviewing repository status.\n";return 0;}
  if(!QFileInfo::exists(mark)){QFile f(mark);if(!f.open(QIODevice::WriteOnly|QIODevice::NewOnly)){err<<"Marker creation failed.\n";return 4;}f.write("J03Curator repository verification marker 28253\n");f.close();}
  int rc=0;run("git",{"add","--","28253.txt"},&rc);if(rc)return rc;
  QString changed=run("git",{"diff","--cached","--name-only"});if(!changed.isEmpty()){out<<run("git",{"commit","-m","J03Curator: ensure marker 28253.txt"},&rc)<<"\n";if(rc)return rc;}
  out<<"Marker checked. NOT pushing or staging other project files without review.\n";return 0;
 }
 if(cmd=="init") {
  if(!QDir(repo()).exists()){err<<"Missing j03-software repository.\n";return 2;}
  for(QString dir:{"Assets","SourceCode","AIInstructions"}) QDir().mkpath(repo()+"/"+dir);
 }
 if(!QDir().mkpath(QFileInfo(dbfile()).absolutePath())){err<<"Cannot create DB directory\n";return 2;}
 auto db=QSqlDatabase::addDatabase("QSQLITE");db.setDatabaseName(dbfile());if(!db.open()){err<<db.lastError().text()<<Qt::endl;return 2;}
 if(!execSql(db,"CREATE TABLE IF NOT EXISTS projects (name TEXT PRIMARY KEY, file_path TEXT NOT NULL, md5 TEXT NOT NULL, copyright_yes INTEGER NOT NULL DEFAULT 0 CHECK(copyright_yes IN (0,1)), useful_yes INTEGER NOT NULL DEFAULT 0 CHECK(useful_yes IN (0,1)), reviewed INTEGER NOT NULL DEFAULT 0 CHECK(reviewed IN (0,1)), created_at TEXT NOT NULL)"))return 2;
 // Enforce yes -> no impossibility even if data is edited outside this CLI.
 execSql(db,"CREATE TRIGGER IF NOT EXISTS irreversible_flags BEFORE UPDATE ON projects WHEN (OLD.copyright_yes=1 AND NEW.copyright_yes=0) OR (OLD.useful_yes=1 AND NEW.useful_yes=0) BEGIN SELECT RAISE(ABORT,'yes flags cannot revert to no'); END");
 // Screenshot candidate selection. No network, no copy, no publishing or approval.
 if(!execSql(db,"CREATE TABLE IF NOT EXISTS screenshot_choices (project TEXT PRIMARY KEY, path TEXT NOT NULL, sha256 TEXT NOT NULL, chosen_at TEXT NOT NULL, approved INTEGER NOT NULL DEFAULT 0)"))return 2;
 if(cmd=="screenshot" && (a.size()==3 || (a.size()==4 && a[3]=="--select-latest"))){
   QSqlQuery exists(db);exists.prepare("SELECT name FROM projects WHERE name=?");exists.addBindValue(a[2]);
   if(!exists.exec()||!exists.next()){err<<"Unknown project. Register it first with: J03Curator add <project> <source-file>\n";return 2;}
   QString root=QDir::homePath()+"/.38bTqEvQc";
   QFileInfo newest;QString name=a[2];
   QDirIterator it(root,QStringList()<<"*.png",QDir::Files,QDirIterator::Subdirectories);
   while(it.hasNext()){
     QFileInfo fi(it.next());
     // Only exact project directory match, case insensitive. Never guess by prefix.
     if(fi.dir().dirName().compare(name,Qt::CaseInsensitive)!=0)continue;
     if(!newest.exists() || fi.lastModified()>newest.lastModified() ||
       (fi.lastModified()==newest.lastModified() && fi.fileName()>newest.fileName()))newest=fi;
   }
   QSqlQuery old(db);old.prepare("SELECT path,sha256,approved FROM screenshot_choices WHERE project=?");old.addBindValue(name);
   bool saved=old.exec()&&old.next();
   out<<"Project: "<<name<<"\n";
   if(saved)out<<"Saved candidate: "<<old.value(0).toString()<<" | approved="<<old.value(2).toInt()<<"\n";
   if(!newest.exists()){out<<"No PNG found in ~/.38bTqEvQc/<project>/; nothing changed.\n";return 0;}
   out<<"Latest local screenshot: "<<newest.absoluteFilePath()<<"\n";
   if(a.size()==3){out<<"Dry run. Use --select-latest to save ONE unapproved candidate. No files copied.\n";return 0;}
   QFile png(newest.absoluteFilePath());if(!png.open(QIODevice::ReadOnly)){err<<"Cannot read screenshot.\n";return 3;}
   QCryptographicHash h(QCryptographicHash::Sha256);if(!h.addData(&png)){err<<"Could not hash screenshot.\n";return 3;}
   QString sha=QString::fromLatin1(h.result().toHex());
   QSqlQuery q(db);q.prepare("INSERT INTO screenshot_choices(project,path,sha256,chosen_at,approved) VALUES(?,?,?,?,0) ON CONFLICT(project) DO UPDATE SET path=excluded.path,sha256=excluded.sha256,chosen_at=excluded.chosen_at,approved=0");
   q.addBindValue(name);q.addBindValue(newest.absoluteFilePath());q.addBindValue(sha);q.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
   if(!q.exec()){err<<q.lastError().text()<<Qt::endl;return 3;}
   out<<"Saved one candidate (unapproved). Previous selection replaced; original PNGs preserved.\n";
   out<<"Nothing copied, staged, committed, pushed or uploaded.\n";return 0;
 }
 if(cmd=="export" && (a.size()==3 || (a.size()==4 && a[3]=="--apply"))){
   const QString name=a[2];
   if(name.isEmpty() || name=="." || name==".." || name.contains('/') || name.contains('\\')){err<<"Invalid project name.\n";return 2;}
   QSqlQuery project(db);project.prepare("SELECT file_path,copyright_yes,useful_yes,reviewed FROM projects WHERE name=?");project.addBindValue(name);
   if(!project.exec() || !project.next()){err<<"Unknown project.\n";return 2;}
   const QString sourceFile=project.value(0).toString();
   const int copyrightFlag=project.value(1).toInt(),useful=project.value(2).toInt(),reviewed=project.value(3).toInt();
   out<<"Project "<<name<<" copyright="<<copyrightFlag<<" useful="<<useful<<" reviewed="<<reviewed<<"\n";
   if(copyrightFlag || !reviewed){err<<"BLOCKED: review project and confirm rights before exporting.\n";return 4;}
   const QString projectDir=QFileInfo(sourceFile).absolutePath();
   if(!QFileInfo::exists(sourceFile)){err<<"Registered source no longer exists.\n";return 4;}
   const QString destSource=repo()+"/SourceCode/"+name;
   const QString destAssets=repo()+"/Assets/"+name;
   const QString destAI=repo()+"/AIInstructions/"+name;
   QStringList sourceNames={"main.cpp","mainwindow.cpp","mainwindow.h","mainwindow.ui","CMakeLists.txt"};
   QStringList copies;
   for(const auto &n:sourceNames)if(QFileInfo::exists(projectDir+"/"+n))copies<<n;
   QSqlQuery q(db);q.prepare("SELECT path,sha256,approved FROM screenshot_choices WHERE project=?");q.addBindValue(name);
   bool hasChoice=q.exec()&&q.next();
   QString image, hash;
   if(hasChoice){image=q.value(0).toString();hash=q.value(1).toString();}
   out<<"Sources: "<<copies.join(", ")<<"\n";
   out<<"Screenshot: "<<(hasChoice?image:"NO SELECTION")<<"\n";
   out<<"Destination: "<<repo()<<"\n";
   if(!a.contains("--apply")){out<<"DRY RUN: nothing copied. To prepare local materials, use export "<<name<<" --apply.\n";return 0;}
   // Explicit publication authority for project documentation only, never store release.
   out<<"Prepare local development-material export for "<<name<<"? [Enter] Continue / [cancel] Cancel: "<<Qt::flush;
   QTextStream input(stdin);
   while(true){
     QString answer=input.readLine();
     if(answer.isNull()){err<<"No input; cancelled.\n";return 5;}
     answer=answer.trimmed();
     if(answer.isEmpty())break;
     if(answer.compare("cancel",Qt::CaseInsensitive)==0){err<<"Cancelled; no changes.\n";return 5;}
     out<<"Press Enter to continue, or type cancel: "<<Qt::flush;
   }
   if(!QDir().mkpath(destSource) || !QDir().mkpath(destAssets) || !QDir().mkpath(destAI)){err<<"Unable to create export directories.\n";return 5;}
   for(const auto &n:copies){
     const QString from=projectDir+"/"+n,to=destSource+"/"+n;
     if(QFileInfo::exists(to)){err<<"Destination exists, refusing overwrite: "<<to<<"\n";return 5;}
   }
   for(const auto &n:copies)if(!QFile::copy(projectDir+"/"+n,destSource+"/"+n)){err<<"Copy failed: "<<n<<"\n";return 5;}
   // Screenshots require exact hash approval and no exclusion. v0.2 does not have a validated
   // shared approval/flag API, therefore no image is copied by this command.
   QFile readme(destAI+"/README.md");
   if(!readme.exists() && readme.open(QIODevice::WriteOnly|QIODevice::NewOnly)){
     QString data="# "+name+" development notes\n\nSource: `SourceCode/"+name+"/`\n\nProject application usefulness: "+(useful?"Yes":"No")+".\n\nThis documentation export does not constitute an application release.\nScreenshot: pending independent screenshotpublisher approval.\n\nBuild with Qt 6 Widgets and CMake using the included source.\n";
     readme.write(data.toUtf8());readme.close();
   }
   out<<"LOCAL EXPORT COMPLETE: "<<copies.size()<<" source files. No screenshot copied (requires screenshotpublisher approval).\n";
   out<<"NOT COMMITTED OR PUSHED. Inspect output and rights before manually using git. No Snap/Flatpak actions.\n";
   return 0;
 }

 // Human-readable next-step handoff for AI assistants. Read-only; never runs recommendations.
 if(cmd=="handoff" && a.size()==3){
   QSqlQuery q(db); q.prepare("SELECT copyright_yes,useful_yes,reviewed FROM projects WHERE name=?");q.addBindValue(a[2]);
   if(!q.exec() || !q.next()){err<<"Unknown project. Register it first with: J03Curator add <project> <source-file>\n";return 2;}
   const int c=q.value(0).toInt(),u=q.value(1).toInt(),r=q.value(2).toInt();
   QString completed="Project "+a[2]+" is registered in J03Curator. Copyright flag="+QString::number(c)+", usefulness flag="+QString::number(u)+", review="+(r?"complete":"pending")+". Local project files have not been verified for public release by this command.";
   QString next;
   if(!r) next="J03Curator review "+a[2];
   else if(c) next="J03Curator plan "+a[2];
   else {
     const QString target=repo()+"/SourceCode/"+a[2];
     if(!QDir(target).exists())next="J03Curator export "+a[2];
     else next="git -C "+repo()+" status --short";
   }
   out<<"Chatgpt or Mistral Vibe, the following has been completed: \""<<completed<<"\" and the next recommendation is to do the following command: \""<<next<<"\". Also, the manual for this program can be found at "<<QDir::homePath()<<"/.jchjtzir/manual.md and the sourcecode for this program can be found at "<<QDir::homePath()<<"/Projects/J03Curator/. Please guide Jeremiah O'Neal on what he should do. Jeremiah will be clueless as to what to do next.\n";
   out<<"This is guidance only. Confirm before any export or publication; no commands have been executed by handoff.\n";
   return 0;
 }
 if(cmd=="init"){out<<"Initialized. All existing files preserved.\n";return 0;}
 if(cmd=="add" && a.size()==4){
   QFile f(a[3]);if(!f.open(QIODevice::ReadOnly)){err<<"Cannot read file.\n";return 2;}
   QString hash=QString::fromLatin1(QCryptographicHash::hash(f.readAll(),QCryptographicHash::Md5).toHex());
   QSqlQuery q(db);q.prepare("INSERT INTO projects(name,file_path,md5,created_at) VALUES (?,?,?,?)");q.addBindValue(a[2]);q.addBindValue(QFileInfo(f).absoluteFilePath());q.addBindValue(hash);q.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
   if(!q.exec()){err<<"Could not add (project may exist): "<<q.lastError().text()<<Qt::endl;return 3;}
   out<<"Added "<<a[2]<<" MD5 "<<hash<<"; flags default NO, reviewed=NO.\n";return 0;
 }
 if(cmd=="list") {QSqlQuery q("SELECT name,md5,copyright_yes,useful_yes,reviewed FROM projects ORDER BY name",db);while(q.next())out<<q.value(0).toString()<<" | MD5="<<q.value(1).toString()<<" | copyright="<<q.value(2).toInt()<<" useful="<<q.value(3).toInt()<<" reviewed="<<q.value(4).toInt()<<"\n";return 0;}
 if(cmd=="set" && a.size()==5){if(a[4].toLower()!="yes"){err<<"Flags only support setting YES. Cannot reset.\n";return 3;}
  QString col=a[3]=="copyright"?"copyright_yes":a[3]=="useful"?"useful_yes":"";if(col.isEmpty()){help();return 2;}
  QSqlQuery q(db);q.prepare("UPDATE projects SET "+col+"=1,reviewed=0 WHERE name=?");q.addBindValue(a[2]);if(!q.exec()||q.numRowsAffected()!=1){err<<"Project not found or update failed.\n";return 3;}
  out<<"Flag set YES permanently. Review reset for safety.\n";return 0;}
 if(cmd=="review" && a.size()==3){QSqlQuery q(db);q.prepare("SELECT copyright_yes,useful_yes FROM projects WHERE name=?");q.addBindValue(a[2]);if(!q.exec()||!q.next()){err<<"Unknown project.\n";return 2;}
  out<<"Confirm classification for "<<a[2]<<": copyright="<<q.value(0).toInt()<<", useful="<<q.value(1).toInt()<<". [Enter] Confirm review / [cancel] Cancel: "<<Qt::flush;
  QTextStream in(stdin);
  while(true){
    QString answer=in.readLine();
    if(answer.isNull()){err<<"No input; cancelled.\n";return 4;}
    answer=answer.trimmed();
    if(answer.isEmpty())break;
    if(answer.compare("cancel",Qt::CaseInsensitive)==0){err<<"Review cancelled; no changes.\n";return 4;}
    out<<"Press Enter to confirm review, or type cancel: "<<Qt::flush;
  }
  QSqlQuery u(db);u.prepare("UPDATE projects SET reviewed=1 WHERE name=?");u.addBindValue(a[2]);u.exec();out<<"Review saved. Publication is still separate and manual.\n";return 0;}
 if((cmd=="plan"||cmd=="instructions") && a.size()==3){QSqlQuery q(db);q.prepare("SELECT copyright_yes,useful_yes,reviewed,file_path FROM projects WHERE name=?");q.addBindValue(a[2]);if(!q.exec()||!q.next()){err<<"Unknown project.\n";return 2;}
  int c=q.value(0).toInt(),u=q.value(1).toInt(),r=q.value(2).toInt();
  out<<"PROJECT "<<a[2]<<" | copyright="<<c<<" useful="<<u<<" reviewed="<<r<<"\n";
  if(c||!r)out<<"HOLD PUBLICATION: unreviewed or restricted; local documentation export blocked.\n";
  else if(!u)out<<"APPLICATION RELEASE BLOCKED: no Snap Store or Flathub. Reviewed development materials may be exported locally with separate authorization; no Git push.\n";
  else out<<"ELIGIBLE FOR MANUAL APPLICATION RELEASE REVIEW (not automatically approved): request description, reusable code, privacy-reviewed screenshot via screenshotpublisher, then Git commit/push and packaging.\n";
  out<<"Next-step handoff: J03Curator handoff "<<a[2]<<"\n";
  out<<"AI workflow: ask whether project is nearly finished. Verify flags with owner. Prepare 4-8-word title, two-paragraph j03.page blog draft, GitHub image/assets/source links, request published blog URL, then Facebook, LinkedIn and one-minute phonetic Cowles Mountain script. Never assert WordPress post is published without confirmation. Refer to teach-ai-latest.txt for historically successful packaging commands; verify current project configuration before using.\n";
  return 0;}
 help();return 2;
}
