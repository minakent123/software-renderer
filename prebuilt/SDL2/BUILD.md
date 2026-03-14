SDL2 prebuilt assets for this repository are organized as:

- `prebuilt/SDL2/macos/arm64/include`
- `prebuilt/SDL2/macos/arm64/debug/lib/libSDL2d.a`
- `prebuilt/SDL2/macos/arm64/release/lib/libSDL2.a`

Regenerate them with:

```bash
./scripts/build_sdl2_prebuilt_macos.sh
```

The script builds from the `third_party/SDL2` submodule and refreshes the checked-in headers and static libraries.
