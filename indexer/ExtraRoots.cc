#include <algorithm>
#include <cstdlib>

#include "spdlog/spdlog.h"

#include "indexer/ExtraRoots.h"

namespace scip_clang {

namespace {

bool validPrefix(std::string_view prefix) {
  if (prefix.empty() || prefix.front() == '/' || prefix.back() == '/') {
    return false;
  }
  size_t start = 0;
  while (start <= prefix.size()) {
    auto end = prefix.find('/', start);
    if (end == std::string_view::npos) {
      end = prefix.size();
    }
    auto segment = prefix.substr(start, end - start);
    if (segment.empty() || segment == "." || segment == "..") {
      return false;
    }
    start = end + 1;
  }
  return true;
}

} // namespace

// static
ExtraRoots ExtraRoots::parse(const std::vector<std::string> &specs) {
  ExtraRoots out;
  for (auto &spec : specs) {
    auto eq = spec.rfind('=');
    std::string dir = eq == std::string::npos ? "" : spec.substr(0, eq);
    std::string prefix = eq == std::string::npos ? "" : spec.substr(eq + 1);
    while (dir.size() > 1 && dir.back() == '/') {
      dir.pop_back();
    }
    if (dir.empty() || dir.front() != '/' || dir == "/" || !validPrefix(prefix)) {
      spdlog::error("--extra-root='{}': want <absolute dir>=<relative prefix>"
                    " (no empty, '.' or '..' segments)",
                    spec);
      std::exit(EXIT_FAILURE);
    }
    out.roots.emplace_back(std::move(dir), std::move(prefix));
    out.specs_.push_back(spec);
  }
  std::stable_sort(out.roots.begin(), out.roots.end(),
                   [](const auto &a, const auto &b) {
                     return a.first.size() > b.first.size();
                   });
  return out;
}

std::optional<std::string>
ExtraRoots::relativize(std::string_view absPath) const {
  for (auto &[dir, prefix] : this->roots) {
    if (absPath.size() > dir.size() + 1 && absPath.starts_with(dir)
        && absPath[dir.size()] == '/') {
      return prefix + std::string(absPath.substr(dir.size()));
    }
  }
  return std::nullopt;
}

std::optional<std::string>
ExtraRoots::absolutize(std::string_view relPath) const {
  const std::pair<std::string, std::string> *best = nullptr;
  for (auto &root : this->roots) {
    auto &prefix = root.second;
    if (relPath.size() > prefix.size() + 1 && relPath.starts_with(prefix)
        && relPath[prefix.size()] == '/'
        && (best == nullptr || prefix.size() > best->second.size())) {
      best = &root;
    }
  }
  if (best == nullptr) {
    return std::nullopt;
  }
  return best->first + std::string(relPath.substr(best->second.size()));
}

} // namespace scip_clang
