#ifndef SCIP_CLANG_EXTRA_ROOTS_H
#define SCIP_CLANG_EXTRA_ROOTS_H

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace scip_clang {

/// Directories outside the project root whose files are indexed as project
/// files under a fixed path prefix, given as --extra-root=<dir>=<prefix>.
///
/// A build writes sources outside the project root (an out-of-source build
/// directory, a code generator's output directory). Without this, such a file
/// is named "<external>/<hash>/<name>" and gets no document, so its
/// definitions are unsearchable. A file under <dir> gets the document path
/// "<prefix>/<path relative to dir>" instead. The prefix is chosen by the
/// caller, which maps it back; scip-clang treats it as an opaque relative
/// path. Matching is at a path boundary, and the longest matching <dir> wins.
class ExtraRoots {
  // (dir without a trailing separator, prefix), longest dir first.
  std::vector<std::pair<std::string, std::string>> roots;
  std::vector<std::string> specs_;

public:
  ExtraRoots() = default;

  /// Parses the --extra-root values; exits with a message on a malformed one.
  static ExtraRoots parse(const std::vector<std::string> &specs);

  bool empty() const { return this->roots.empty(); }

  /// The prefixed relative path of absPath, if it lies strictly under a root.
  std::optional<std::string> relativize(std::string_view absPath) const;

  /// The absolute path of a prefixed relative path, if it has a root's prefix.
  std::optional<std::string> absolutize(std::string_view relPath) const;

  /// The values as given, for forwarding to workers.
  const std::vector<std::string> &specs() const { return this->specs_; }
};

} // namespace scip_clang

#endif // SCIP_CLANG_EXTRA_ROOTS_H
