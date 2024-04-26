rm -r src/*
rm -r build/*
mkdir src
mkdir build

asn1c lte-rrc-15.6.0.asn1 -D src -no-gen-OER -fcompound-names -no-gen-example -fno-include-deps
gcc -g -Isrc -I. -o build/enb main_enb.c RRC_coder.c src/*.c -lsctp -DPDU=UEContextReleaseCommand -DASN_DISABLE_OER_SUPPORT
gcc -g -Isrc -I. -o build/ub main_ub.c RRC_coder.c src/*.c -lsctp -DPDU=UEContextReleaseCommand -DASN_DISABLE_OER_SUPPORT


