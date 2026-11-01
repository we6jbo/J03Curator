# J03Curator: Jim Schlorf's Dell and Mistral Guide

## October 2026 development handoff

Prepared for Jim Schlorf.

J03Curator is an AI-assisted software project management and
publication preparation utility developed by Jeremiah B. O'Neal.

Its goal is to help developers organize projects, review copyright
and usefulness, manage screenshots, prepare reusable source code,
and coordinate publication with AI assistants.

This document explains how Jim can evaluate and extend J03Curator
on a Dell computer using Mistral and ChatGPT.

## 1. Understand the current project

J03Curator is written primarily in C++ using Qt and CMake.
It provides command-line project management functionality.

The October 2026 version includes:
- Project registration and status tracking.
- Copyright and usefulness review.
- AI handoff instructions.
- Local documentation export.
- Screenshot candidate selection.
- Completion-phase guidance.
- Publication approval safeguards.

The application is not yet a complete automatic publishing system.
Do not assume a feature exists merely because it is documented as
a future goal.

## 2. Running on Jim's Dell

First determine whether Jim's Dell runs Linux, Windows, or another
operating system.

For Linux:
1. Install an appropriate C++ compiler, Qt development libraries,
   CMake, and Git.
2. Obtain the J03Curator source repository.
3. Ask Mistral to examine CMakeLists.txt and the source files.
4. Build the program using the instructions appropriate for the
   installed Qt version and operating system.
5. Run available help and status commands.
6. Verify actual behavior before using project data.

For Windows:
1. Install Qt, CMake, and a compatible C++ toolchain.
2. Ask ChatGPT or Mistral to inspect platform-specific assumptions.
3. Adapt Linux-only filesystem paths and subprocess calls.
4. Build and test without changing the original Linux behavior.

The original developer's /home/we6jbo paths must not be assumed
to exist on Jim's Dell.

Keep a separate development copy and preserve the original files,
Git history, database contents, and backups.

## 3. How Jim can use Mistral

Mistral can assist with:
- Reading the project source and documentation.
- Explaining supported J03Curator commands.
- Diagnosing compilation or runtime errors.
- Developing missing features.
- Writing tests and checking results.
- Preparing changes for review and Git commits.

Start with README.md, manual.md, AI_WORKFLOW.md,
SCREENSHOT_POLICY.md, and EXPORT_POLICY.md.

Run J03Curator help or consult the actual manual to determine the
commands supported by the installed version.

Never assume Mistral has executed a command unless it has access
to the computer and reports actual execution results.

## 4. Improvements needed to approach 10/10 usefulness

The October 2026 usefulness assessment for Jeremiah's workflow
was approximately 8/10. This is a subjective assessment, not a
guaranteed or independently certified score.

Recommended improvements:

A. Reliable screenshot capture
   Add a screenshot button to graphical applications, or a suitable
   output-capture function to command-line applications.

B. Reliable screenshot registration
   Save screenshots in a project-specific directory and register
   them with infoartrecorder when available.

C. Portable configuration
   Replace hard-coded developer-specific paths with configurable
   paths and platform-appropriate defaults.

D. Complete integration tests
   Test project registration, review, screenshot selection,
   documentation export, and failure handling.

E. Documentation consistency
   Ensure the displayed application version, manual, README,
   and actual commands agree.

F. Safe publication workflow
   Support verified, explicitly approved publication operations
   without automatically uploading private or unapproved content.

G. Portability
   Test on Jim's Dell and on another supported operating system.

H. Recovery and backups
   Preserve database history, existing files, and Git commits.
   Test recovery after interrupted operations.

I. Licensing
   Verify the rights and licenses of source files and dependencies.
   Do not invent permissions or claim unverified license terms.

J. Independent usability testing
   Ask another developer to follow the documentation without
   relying on the original developer's environment.

A 10/10 usefulness rating should be earned through demonstrated
reliability, portability, usability, and testing.

## 5. Screenshot integration architecture

Application screenshot button or capture function
    |
    v
Save a uniquely named local PNG
    |
    v
Register using infoartrecorder
    |
    v
Review through screenshotpublisher
    |
    v
Select candidate with J03Curator
    |
    v
Obtain explicit owner approval before publication

For Jeremiah's T14, the established screenshot location is:

/home/we6jbo/.38bTqEvQc/<lowercase-project-name>/

The output should normally be owned by the user creating it.

For Jim's Dell, use Jim's own home directory and account.
Do not hard-code we6jbo ownership on his computer.

Capture should be silent, without popup dialogs.
A missing infoartrecorder installation must not prevent
the application from retaining a successfully saved screenshot.

Registration is not the same as publication approval.

At most one approved screenshot per project should be published
under the original project's publication policy.

## 6. Ready-to-copy ChatGPT request for Jim

Jim can paste the following request into ChatGPT:

"Please examine my copy of the J03Curator project and its
README.md, manual.md, AI_WORKFLOW.md, SCREENSHOT_POLICY.md,
EXPORT_POLICY.md, CMakeLists.txt, and C++ source files.

I am using a Dell computer with Mistral. First help me identify
my operating system and available development tools.

I want to improve J03Curator from its October 2026 baseline
toward a genuinely tested 10/10 usefulness rating.

Identify the features that are missing, incomplete, untested,
or dependent on Jeremiah's Manjaro computer.

Create the necessary source files, CMake changes, tests,
configuration files, and documentation, but preserve all
existing functionality and data.

Prioritize portable paths, screenshot capture and registration,
clear copyright review, publication approval, error handling,
and consistency between software and documentation.

Give me downloadable source packages or precise patches,
installation instructions, and commands to test each change.

Do not claim a test succeeded unless it was actually performed.
Do not automatically publish code, screenshots, or other files.

Explain the results in plain English, and recommend one
concrete next step at a time.

Before declaring the software 10/10, show evidence that
important features work on my Dell and explain any remaining
limitations or unverified assumptions."

## 7. Using AI-generated improvements safely

Jim should implement and verify improvements incrementally.

For each change:
1. Inspect the current Git status.
2. Preserve a recoverable baseline.
3. Review the AI's proposed changes.
4. Build the application.
5. Run relevant tests.
6. Review filesystem and database effects.
7. Commit only verified files.
8. Keep publication behind an explicit approval step.

Do not let an AI overwrite the entire repository simply to add
a missing feature.

## 8. October 2026 baseline

Jeremiah's recorded midpoint commit:

0574998 - end of october midpoint

This guide is intended as a follow-up documentation commit
named "end of october 2026".

For development in November, compare improvements against
the actual Git baseline and verify all new functionality.

## 9. Important distinction

J03Curator, infoartrecorder, and screenshotpublisher serve
different purposes.

J03Curator coordinates project reviews and publication planning.
infoartrecorder registers existing screenshot files.
screenshotpublisher handles image approval and staging.

Screenshot capture itself must be implemented by the application
or a separate capture mechanism.

These components may be integrated, but their individual security
and publication responsibilities should remain clear.

## Final objective

Make J03Curator useful not only to its original developer,
but also to Jim Schlorf and other developers using different
computers and AI assistants.

The goal is a trustworthy, well-tested, portable workflow,
not merely a higher numerical rating.
