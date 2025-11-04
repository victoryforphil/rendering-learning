# This file defines the Bazel module extension for managing spdlog as a dependency.
# spdlog is a header-only logging library, so no build steps are required.

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def _spdlog_deps_impl(ctx):
    git_repository(
        name = "spdlog",
        remote = "https://github.com/gabime/spdlog.git",
        commit = "486b55554f11c9cccc913e11a87085b2a91f706f",
        build_file_content = """
package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # BSD-2-Clause

cc_library(
    name = "spdlog",
    hdrs = glob(["include/**"], allow_empty = False),
    includes = ["include"],
    strip_include_prefix = "include",
)
""",
    )

spdlog_extension = module_extension(
    implementation = _spdlog_deps_impl,
)