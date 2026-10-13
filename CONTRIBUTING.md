# Contributing

Thanks for your interest in Spider! Contributions are welcome as long as they follow the rules below.

# AI Policy

**Massive, meaningless AI-generated code changes are not accepted.**

Every line in the kernel needs to be understood by the people who wrote it and the people who maintain it. AI slop breaks that.

What counts as slop:

- Huge diffs that touch many files with no clear purpose.
- Code that looks plausible but that the author can't explain.
- Rewrites, refactors, or "cleanups" that nobody asked for.
- Extra abstractions, wrappers, and layers that add complexity without solving a real problem.
- Boilerplate, filler comments, and bloated documentation that say nothing useful.
- Fixes for problems that don't exist, or "improvements" that don't improve anything.

PRs that look like AI slop will be closed without review.

# PRs

- Keep your PR in Draft until it's ready for review.
- Keep your PR simple and do only one thing at a time. Split unrelated changes into separate PRs.
- Open an issue first for large changes (new subsystems, architecture changes, big refactors) so we can agree on the approach before you write code.
- Make sure the kernel builds cleanly and boots in an emulator (e.g. QEMU) before marking the PR ready for review.
- Follow the existing code style. Match the naming, indentation, and comment style of the surrounding code.
- Write clear commit messages: a short summary line, then more detail in the body if needed.
- Don't commit build artifacts, binaries, or ISO images.
- Be ready to explain your code. Reviewers may ask you why you made a particular choice, and you are expected to know.

# Issues

- Anyone can open an issue.
- Search existing issues first to avoid duplicates.
- For bug reports, include:
  - What you expected to happen and what actually happened
  - Steps to reproduce
  - Your environment (emulator or real hardware, toolchain, host OS)
  - Any relevant serial or debug output
- For feature requests, explain what you want and why it fits the project.
- Keep each issue focused on one problem or one request.

# Code of Conduct

- Be respectful and constructive. No harassment, insults, or personal attacks.
- Criticize code, not people.
- Maintainers have the final say on what gets merged.