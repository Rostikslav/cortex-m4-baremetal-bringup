#!/usr/bin/env bash
set -euo pipefail
exec JLinkGDBServer -if SWD -device NRF52832_XXAA -speed 4000 -port 2331