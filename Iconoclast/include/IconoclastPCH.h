#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#pragma warning(push)
#pragma warning(disable : 5039)
#include <windows.h>
#pragma warning(pop)
#endif

#include <algorithm>  // IWYU pragma: export
#include <functional> // IWYU pragma: export
#include <iostream>   // IWYU pragma: export
#include <memory>     // IWYU pragma: export
#include <utility>    // IWYU pragma: export

#include <sstream>       // IWYU pragma: export
#include <string>        // IWYU pragma: export
#include <unordered_map> // IWYU pragma: export
#include <unordered_set> // IWYU pragma: export
#include <vector>        // IWYU pragma: export
