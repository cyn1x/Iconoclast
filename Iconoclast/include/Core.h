/**
 * @file Core.h
 * Abstraction of library import and export definitions.
 * Used to include in other header files which require their functions to be
 * exported, or source files for functions to be imported.
 */

#pragma once

#ifndef DEFS_H
#define DEFS_H

#ifdef _WIN32
#pragma warning(disable : 5045)
#endif // _WIN32

#ifdef _WIN32
#ifdef ICONOCLAST_EXPORTS
#define ICONOCLAST_API __declspec(dllexport)
#else
#define ICONOCLAST_API __declspec(dllimport)
#endif // ICONOCLAST_EXPORTS
#elif __linux__
#define ICONOCLAST_API __attribute__((visibility("default")))
#else
#define ICONOCLAST_API
#endif // _WIN32

#endif // DEFS_H
