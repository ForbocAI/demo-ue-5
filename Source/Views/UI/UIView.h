// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

/**
 * @file RuntimeUIView.h
 * @brief View-layer include facade for runtime UI presentation classes.
 *
 * Architecture: Consumers should import actions/thunks/selectors from feature
 * domains and import this facade only for display types. The facade contains no
 * reducers, store access, or functional-core substitutions.
 */
#include "Views/Chat/ChatView.h"
