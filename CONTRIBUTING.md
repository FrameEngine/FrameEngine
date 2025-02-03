# Contributing to FrameEngine

Thank you for considering contributing to FrameEngine! Here’s how you can help:

## How to Contribute

1. **Fork the repository.**
2. **Create a branch** for your changes.
    Here some examples of good branch names:
    - `feature/add-opengl-renderer`
    - `bugfix/fix-physics-integration`
    - `docs/update-contribution-guide`

3. **Make your changes** following our [coding style](#code-style-guidelines).
4. **Build the Project** using `make rebuild`
5. **Run tests:** Ensure all tests pass by running `make test`.
6. **Commit your changes** Make sure your commit **messages are clear** and **reference issues** if applicable.
    Example of good commit header: `git commit -m "Fix gravity calculation in PhysicsSystem (#42)"`
7. **Open a Pull Request:** Describe your changes following [PULL_REQUEST_TEMPLATE](./PULL_REQUEST_TEMPLATE.md)

## Code Style Guidelines

- Naming Conventions: Use names consistent with existing code (e.g., `MeshComponent`, `PhysicsComponent`, etc.).
- Follow commit message conventions: a short summary, a blank line, and then a detailed description that explains *What* and **Why**.
- Update or add documentation comments to your code following **Doxygen-style** standarts (Can read more [here](https://www.doxygen.nl/manual/docblocks.html))

## Reporting Issues

To report a bug use the [Issue Template](./ISSUE_TEMPLATE.md)

Before submitting a bug report, please:
- Search the issue tracker for similar issues.
- Include clear steps to reproduce, details about your system (compiler version, OS, etc.), and any error logs.
- Include any relevant logs or error messages.
- Provide screenshots if applicable

## Proposing New Features

If you’d like to propose a new feature, please **open a discussion before submitting a PR** to avoid redundant work.

### How to suggest a Feature

1. Check if a similar feature request already exists
2. Celarly describe:
  - What the feature should do
  - Why is it useful
  - Possible implementation approach is highly appreciated
3. Submit your request in the Issues section using the "Feature Request" template.

Thank you for helping improve FrameEngine!

