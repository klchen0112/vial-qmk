{
  description = "Flake for building qmk firmware";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        # change to your liking
        keyboard_map = {
          "motev3" = "kapx/motev3/56keys/round_3389";
          "charybdis4x6" = "bastardkb/charybdis/4x6/v2/splinky_3";
        };
        keymap = "engrammer";

        mkFirmware  = name: keyboard_path : pkgs.stdenv.mkDerivation {
          pname = "qmk-firmware-${name}";
          version = "0.1.0";
          src = ./.;

          # 必须包含子模块
          phases = [ "buildPhase" ];
          # 必须包含子模块
          submodules = true;

          nativeBuildInputs = [ pkgs.qmk pkgs.gcc-arm-embedded pkgs.python3 ];

          buildPhase = ''
            make -C $src BUILD_DIR=`pwd`/.build COPY=echo -j8 ${keyboard_path}:${keymap}
            mkdir $out
            cp -r .build/* $out/
          '';
        };
        mkShell = name: path :
        ''
          build_${name}() {
            qmk compile -kb ${path} -km engrammer
          }
          flash_${name}() {
            qmk flash  -kb ${path} -km engrammer
          }
        '';
      in
      {
        # build with `nix build '.?submodules=1'`
        packages = builtins.mapAttrs (name: path: mkFirmware name path) keyboard_map;

        # Get a devshell with `nix develop` and then use `build` and `flash`.
        # Build dir is taken as a param of `build` and `flash`,
        # e.g. to flash with the output of `nix build '.?submodules=1'` do `flash result`.
        devShell = pkgs.mkShell {
          buildInputs = [ pkgs.qmk ];
          shellHook = pkgs.lib.concatStringsSep "\n" (pkgs.lib.mapAttrsToList (name: value:
            mkShell name value
          ) keyboard_map);
        };
      }
  );
}
