import re

with open("Plugins/ForbocAI_SDK/Plugins/ForbocAI_SDK/scripts/verify-ue-parity.sh", "r") as f:
    content = f.read()

# Add QUICK_MODE logic
quick_mode_logic = '''
QUICK_MODE=0
for arg in "$@"; do
  if [ "$arg" = "--quick" ]; then
    QUICK_MODE=1
  fi
done
'''

# Find where SCRIPT_DIR is set and insert the logic
content = re.sub(
    r'(SCRIPT_DIR="\$\(cd "\$\(dirname "\$0"\)" && pwd\)")',
    r'\1' + quick_mode_logic,
    content
)

# Replace the run_check calls for phases 4 and 5
old_phase4 = '''# ── Phase 4: Product Boundary ──
run_check "Product Boundary (game-agnostic audit)" \\
  "$SCRIPT_DIR/check-product-boundary.sh"'''
new_phase4 = '''# ── Phase 4: Product Boundary ──
if [ "$QUICK_MODE" -eq 1 ]; then
  echo -e "\\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo -e "${CYAN}[SKIPPED] Product Boundary (game-agnostic audit)${NC}"
  echo -e "${YELLOW}Skipped in --quick mode${NC}"
else
  run_check "Product Boundary (game-agnostic audit)" \\
    "$SCRIPT_DIR/check-product-boundary.sh"
fi'''

old_phase5 = '''# ── Phase 5: API Contract Parity ──
run_check "Canonical Contract Parity (API test-game contract)" \\
  "$SCRIPT_DIR/check-api-contract-parity.py"'''
new_phase5 = '''# ── Phase 5: API Contract Parity ──
if [ "$QUICK_MODE" -eq 1 ]; then
  echo -e "\\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo -e "${CYAN}[SKIPPED] Canonical Contract Parity (API test-game contract)${NC}"
  echo -e "${YELLOW}Skipped in --quick mode${NC}"
else
  run_check "Canonical Contract Parity (API test-game contract)" \\
    "$SCRIPT_DIR/check-api-contract-parity.py"
fi'''

content = content.replace(old_phase4, new_phase4)
content = content.replace(old_phase5, new_phase5)

with open("Plugins/ForbocAI_SDK/Plugins/ForbocAI_SDK/scripts/verify-ue-parity.sh", "w") as f:
    f.write(content)

