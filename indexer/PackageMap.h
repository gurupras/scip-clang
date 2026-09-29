#ifndef SCIP_CLANG_PACKAGE_MAP_H
#define SCIP_CLANG_PACKAGE_MAP_H

#include <string_view>

#include "absl/container/flat_hash_map.h"

#include "clang/Basic/SourceLocation.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/StringSaver.h"

#include "indexer/FileMetadata.h"
#include "indexer/FileSystem.h"
#include "indexer/Path.h"

namespace scip_clang {

/// Map tracking path->(name, version) which persists across TUs.
///
/// Modifiers internal structures to cache lookups.
/// Where a package map entry lies relative to the project root (both real,
/// absolute paths, compared by component).
enum class EntryPlacement {
  Outside,  // shares no subtree with the project
  Project,  // is the project root
  Ancestor, // contains the project root
  Inside,   // lies within the project
};

EntryPlacement placeEntry(std::string_view entry, std::string_view projectRoot);

class PackageMap final {
  llvm::BumpPtrAllocator storage;
  llvm::UniqueStringSaver interner;
  absl::flat_hash_map<AbsolutePathRef, PackageMetadata> map;

  absl::flat_hash_set<std::string_view> warnedBadPaths;

  RootPath projectRootPath;

  bool isTesting;
  bool partial;

public:
  /// Populate a PackageMap from a file with entries in the format:
  ///
  /// {"directory": "<some path>", "package": "name@version" }
  ///
  /// Changes to fallback behavior if packageMapPath is empty. With \p partial,
  /// the map need not name the project: a file no entry covers gets no
  /// package, as without a map.
  PackageMap(const RootPath &projectRootPath, const StdPath &packageMapPath,
             bool isTesting, bool partial = false);

  /// Check if we have package information for \p filepath.
  ///
  /// Non-const as it caches intermediate results produced during lookups.
  std::optional<PackageMetadata> lookup(AbsolutePathRef filepath);

private:
  std::string_view store(std::string_view);

  bool checkPathIsNormalized(AbsolutePathRef);

  void populate(const StdPath &packageMapPath);
};

} // namespace scip_clang

#endif