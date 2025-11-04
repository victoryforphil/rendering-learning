# rendering-learning# rendering-learning# rendering-learning



A learning project for graphics rendering with C++ and OpenGL, built with Bazel.



## OverviewA learning project for graphics rendering with C++ and OpenGL, built with Bazel.## Using Bazel with Bzlmod (teaching walkthrough)



This project demonstrates:

- Modern OpenGL rendering with GLFW and GLAD

- C++ development with Bazel build system## OverviewBazel 8+ manages external dependencies with Bzlmod. This repo is configured to use Bzlmod via `MODULE.bazel` (the modern replacement for `WORKSPACE`).

- Dependency management using Bazel's Bzlmod

- Cross-platform graphics programming



## Quick StartThis project demonstrates:### What changed and why



### Prerequisites- Modern OpenGL rendering with GLFW and GLAD



- Bazel 8+ (required for Bzlmod support)- C++ development with Bazel build system- External deps (Python packages, third‑party C/C++ libs) are now declared in `MODULE.bazel` instead of `WORKSPACE`.

- C++ compiler (Clang/GCC)

- Python 3.11+ (for build tools)- Dependency management using Bazel's Bzlmod- Python packages from `requirements.txt` are exposed to BUILD files via a generated repo named `@pip_requirements`.



### Building- Cross-platform graphics programming- GLFW and GLM are fetched from Git using `git_repository` declared in `MODULE.bazel`, each wired to a local `*.BUILD` wrapper that defines simple `cc_library` targets.



Build all targets:

```bash

bazel build //...## Quick StartThis makes dependency management explicit, reproducible, and compatible with Bazel 8+ (and required in Bazel 9).

```



Build a specific example:

```bash### Prerequisites### Key files and snippets

bazel build //examples:hello_triangle

```



### Running Examples- Bazel 8+ (required for Bzlmod support)1) `MODULE.bazel` (external deps live here):



```bash- C++ compiler (Clang/GCC)

bazel run //examples:hello_triangle

bazel run //examples:hello_log- Python 3.11+ (for build tools)```bzl

```

module(name = "rendering-learning")

## Project Structure

### Building

```

rendering-learning/bazel_dep(name = "rules_python", version = "0.31.0")

├── examples/          # Example programs

│   ├── hello_log.ccBuild all targets:

│   └── hello_triangle.cc

├── glad/              # OpenGL loader generator```bashpython = use_extension("@rules_python//python/extensions:python.bzl", "python")

├── notes/             # Documentation

│   └── build.md       # Build system detailsbazel build //...python.toolchain(python_version = "3.11")

├── MODULE.bazel       # Bazel dependencies (Bzlmod)

└── BUILD              # Build configuration```

```

pip = use_extension("@rules_python//python/extensions:pip.bzl", "pip")

## Dependencies

Build a specific example:pip.parse(

- **GLFW** - Windowing and input handling

- **GLM** - OpenGL Mathematics```bash		hub_name = "pip_requirements",

- **GLAD** - OpenGL loader

- **spdlog** - Logging librarybazel build //examples:hello_triangle		python_version = "3.11",



All dependencies are managed via Bazel's MODULE.bazel. See [`notes/build.md`](notes/build.md) for details.```		requirements_lock = "//:requirements.txt",



## Development)



### VS Code Setup### Running Examplesuse_repo(pip, "pip_requirements")



This project is configured for optimal C++ development using **clangd** as the language server, which provides:

- Fast and accurate IntelliSense

- Real-time error detection with clang-tidy```bashgit_repository = use_repo_rule("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

- Code formatting and refactoring

- Better integration with compile_commands.jsonbazel run //examples:hello_triangle



The workspace includes:bazel run //examples:hello_loggit_repository(

- Automatic clangd configuration (`.clangd`)

- Build and run tasks```		name = "glfw",

- Debug configurations with CodeLLDB

		remote = "https://github.com/glfw/glfw.git",

See [`notes/build.md`](notes/build.md) for VS Code extension recommendations and setup details.

## Project Structure		commit = "8d7e5cdb49a1a5247df612157ecffdd8e68923d2",

### Using clangd

		build_file = "//:glfw.BUILD",

The project uses `compile_commands.json` for accurate code intelligence. After adding or modifying dependencies, refresh it:

```bash```)

bazel run @hedron_compile_commands//:refresh_all

```rendering-learning/



The clangd language server will automatically pick up the changes and update IntelliSense.├── examples/          # Example programsgit_repository(



**Note:** The default C/C++ extension IntelliSense is disabled in favor of clangd for better performance with Bazel projects.│   ├── hello_log.cc		name = "glm",



## Documentation│   └── hello_triangle.cc		remote = "https://github.com/g-truc/glm.git",



- **[Build System](notes/build.md)** - Detailed Bazel and Bzlmod documentation├── glad/              # OpenGL loader generator		commit = "658d8960d081e0c9c312d49758c7ef919371b428",

- **[Examples](examples/)** - Sample rendering programs

├── notes/             # Documentation		build_file = "//:glm.BUILD",

## License

│   └── build.md       # Build system details)

See [LICENSE](LICENSE) file for details.

├── MODULE.bazel       # Bazel dependencies (Bzlmod)```

└── BUILD              # Build configuration

```2) `glad/BUILD` (how Python deps are loaded under Bzlmod):



## Dependencies```bzl

load("@pip_requirements//:requirements.bzl", "requirement")

- **GLFW** - Windowing and input handlingload("@rules_python//python:defs.bzl", "py_binary")

- **GLM** - OpenGL Mathematics

- **GLAD** - OpenGL loaderpy_binary(

- **spdlog** - Logging library		name = "run_glad",

		srcs = ["run_glad.py"],

All dependencies are managed via Bazel's MODULE.bazel. See [`notes/build.md`](notes/build.md) for details.		deps = [requirement("glad")],

)

## Development```



### VS Code Setup3) `examples/BUILD` (C++ example using external C/C++ libs):



This project includes VS Code configuration for C++ development with Bazel:```bzl

- IntelliSense via clangdcc_binary(

- Build tasks	name = "hello_triangle",

- Debug configurations	srcs = ["hello_triangle.cc"],

	deps = [

See [`notes/build.md`](notes/build.md) for VS Code extension recommendations and setup details.		"@glfw//:glfw",

		"@glm//:glm",

### Refreshing IntelliSense		"//glad:glad",

	],

After adding or modifying dependencies:)

```bash```

bazel run @hedron_compile_commands//:refresh_all

```### Try it



## DocumentationBuild the example:



- **[Build System](notes/build.md)** - Detailed Bazel and Bzlmod documentation```bash

- **[Examples](examples/)** - Sample rendering programsbazel build //examples:hello_triangle

```

## License

Common gotcha: If you see an error like “No repository visible as '@requirements'”, it means a BUILD file is still loading the old pip repo name. Under Bzlmod we load from `@pip_requirements//:requirements.bzl` instead.

See [LICENSE](LICENSE) file for details.

### How Python deps work now

- `requirements.txt` is parsed by the `rules_python` pip extension at module resolution time.
- It generates a repository `@pip_requirements` that contains a `requirements.bzl` with a `requirement()` macro.
- In BUILD files, use `deps = [requirement("package-name")]`.

### How non-BCR C/C++ deps work

- For projects without a Bazel module on the Bazel Central Registry (BCR), we declare them via `git_repository` in `MODULE.bazel`.
- We supply a local wrapper BUILD file (e.g., `glfw.BUILD`, `glm.BUILD`) to expose simple `cc_library` targets.

That’s it—you’re on Bzlmod. If you later move a dependency to the BCR, replace the `git_repository` with a simple `bazel_dep(...)`.

## Using Clang with Bazel (macOS)

This repository includes a `.bazelrc` that configures Bazel to use the system Clang/Clang++ on macOS. The file sets the `CC` and `CXX` action environment variables to `/usr/bin/clang` and `/usr/bin/clang++` respectively, and enforces a modern C++ standard.

If you want to override that for a single build, you can pass environment variables directly:

```bash
CC=/usr/bin/clang CXX=/usr/bin/clang++ bazel build //examples:hello_triangle
```

Or edit `.bazelrc` to point to a different clang installation (for example an Xcode toolchain or homebrew-installed clang).