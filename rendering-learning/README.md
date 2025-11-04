# rendering-learning

## Using Bazel with Bzlmod (teaching walkthrough)

Bazel 8+ manages external dependencies with Bzlmod. This repo is configured to use Bzlmod via `MODULE.bazel` (the modern replacement for `WORKSPACE`).

### What changed and why

- External deps (Python packages, third‑party C/C++ libs) are now declared in `MODULE.bazel` instead of `WORKSPACE`.
- Python packages from `requirements.txt` are exposed to BUILD files via a generated repo named `@pip_requirements`.
- GLFW and GLM are fetched from Git using `git_repository` declared in `MODULE.bazel`, each wired to a local `*.BUILD` wrapper that defines simple `cc_library` targets.

This makes dependency management explicit, reproducible, and compatible with Bazel 8+ (and required in Bazel 9).

### Key files and snippets

1) `MODULE.bazel` (external deps live here):

```bzl
module(name = "rendering-learning")

bazel_dep(name = "rules_python", version = "0.31.0")

python = use_extension("@rules_python//python/extensions:python.bzl", "python")
python.toolchain(python_version = "3.11")

pip = use_extension("@rules_python//python/extensions:pip.bzl", "pip")
pip.parse(
		hub_name = "pip_requirements",
		python_version = "3.11",
		requirements_lock = "//:requirements.txt",
)
use_repo(pip, "pip_requirements")

git_repository = use_repo_rule("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
		name = "glfw",
		remote = "https://github.com/glfw/glfw.git",
		commit = "8d7e5cdb49a1a5247df612157ecffdd8e68923d2",
		build_file = "//:glfw.BUILD",
)

git_repository(
		name = "glm",
		remote = "https://github.com/g-truc/glm.git",
		commit = "658d8960d081e0c9c312d49758c7ef919371b428",
		build_file = "//:glm.BUILD",
)
```

2) `glad/BUILD` (how Python deps are loaded under Bzlmod):

```bzl
load("@pip_requirements//:requirements.bzl", "requirement")
load("@rules_python//python:defs.bzl", "py_binary")

py_binary(
		name = "run_glad",
		srcs = ["run_glad.py"],
		deps = [requirement("glad")],
)
```

3) `examples/BUILD` (C++ example using external C/C++ libs):

```bzl
cc_binary(
	name = "hello_triangle",
	srcs = ["hello_triangle.cc"],
	deps = [
		"@glfw//:glfw",
		"@glm//:glm",
		"//glad:glad",
	],
)
```

### Try it

Build the example:

```bash
bazel build //examples:hello_triangle
```

Common gotcha: If you see an error like “No repository visible as '@requirements'”, it means a BUILD file is still loading the old pip repo name. Under Bzlmod we load from `@pip_requirements//:requirements.bzl` instead.

### How Python deps work now

- `requirements.txt` is parsed by the `rules_python` pip extension at module resolution time.
- It generates a repository `@pip_requirements` that contains a `requirements.bzl` with a `requirement()` macro.
- In BUILD files, use `deps = [requirement("package-name")]`.

### How non-BCR C/C++ deps work

- For projects without a Bazel module on the Bazel Central Registry (BCR), we declare them via `git_repository` in `MODULE.bazel`.
- We supply a local wrapper BUILD file (e.g., `glfw.BUILD`, `glm.BUILD`) to expose simple `cc_library` targets.

That’s it—you’re on Bzlmod. If you later move a dependency to the BCR, replace the `git_repository` with a simple `bazel_dep(...)`.