#pragma once

/**
 * @file RuntimeUI.h
 * @brief View-layer include facade for runtime UI presentation classes.
 *
 * Architecture: Consumers should import actions/thunks/selectors from feature
 * domains and import this facade only for display types. The facade contains no
 * reducers, store access, or functional-core substitutions.
 */
#include "Views/RuntimeChatWidget.h"
