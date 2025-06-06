#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif

#pragma warning(push)
#pragma warning(disable : 5039)
#include <windows.h>

#pragma warning(disable : 4061)
#pragma warning(disable : 4365)
#pragma warning(disable : 4668)
#pragma warning(disable : 4820)
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#pragma warning(pop)
#endif

#include <algorithm>  // IWYU pragma: export
#include <functional> // IWYU pragma: export
#include <iostream>   // IWYU pragma: export
#include <memory>     // IWYU pragma: export
#include <utility>    // IWYU pragma: export

#include <cstdint>       // IWYU pragma: export
#include <filesystem>    // IWYU pragma: export
#include <fstream>       // IWYU pragma: export
#include <sstream>       // IWYU pragma: export
#include <string>        // IWYU pragma: export
#include <unordered_map> // IWYU pragma: export
#include <unordered_set> // IWYU pragma: export
#include <vector>        // IWYU pragma: export
