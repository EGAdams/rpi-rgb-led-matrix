#sudo ./examples-api-use/minimal-example --led-chain=4
#sudo examples-api-use/minimal-example --led-chain=4 --led-multiplexing=2 --led-pixel-mapper="U-mapper"
#sudo examples-api-use/minimal-example --led-chain=8 --led-multiplexing=2 --led-pixel-mapper="U-mapper" --led-brightness=1
#sudo examples-api-use/pixel-mover --led-chain=8 --led-multiplexing=2 --led-pixel-mapper="U-mapper" --led-brightness=15 --led-gpio-mapping=adafruit-hat -t 128
#sudo examples-api-use/minimal-example --led-brightness=20 --led-chain=16
#sudo examples-api-use/minimal-example --led-brightness=1 --led-chain=8 --led-pixel-mapper="U-mapper"
#sudo examples-api-use/demo --led-multiplexing=2 --led-brightness=20 --led-chain=8 --led-pixel-mapper="U-mapper" --led-gpio-mapping=adafruit-hat -D 9
#sudo examples-api-use/minimal-example --led-gpio-mapping=regular --led-brightness=20 --led-chain=8 --led-pixel-mapper="164-mapper" --led-multiplexing=0
#sudo examples-api-use/draw_random_pix
#sudo examples-api-use/panel_test --led-no-hardware-pulse
#sudo examples-api-use/panel_test --led-no-hardware-pulse
#sudo examples-api-use/panel_test --led-no-hardware-pulse --led-multiplexing=1 --led-pixel-mapper="Rotate:270;Mirror:V"
#  Single HUB75 panel (64x64) comes out of the box working with below command.
#  led-multiplexing is set to 1 and is also named "Stripe"
#  So we have 3 filters:
#  3. Stripe
#  1. Rotate:270
#  2. Mirror:V

#below command worked on april 24, 2023
#sudo examples-api-use/demo -D 9 --led-no-hardware-pulse --led-multiplexing=1 --led-pixel-mapper="Rotate:180;264-Mapper" --led-gpio-mapping=regular --led-chain=1 --led-cols=64 --led-rows=64 --led-parallel=2 --led-brightness=20
#sudo examples-api-use/text-example -f fonts/mspgothic_042623.bdf --led-no-hardware-pulse --led-multiplexing=1 --led-gpio-mapping=regular --led-parallel=2 --led-pixel-mapper="Rotate:180;264-Mapper" --led-cols=64 --led-rows=64 --led-brightness=35

sudo tennis-game -f fonts/mspgothic_042623.bdf --led-no-hardware-pulse --led-multiplexing=1 --led-gpio-mapping=regular --led-parallel=2 --led-pixel-mapper="Rotate:180;264-Mapper" --led-cols=64 --led-rows=64 --led-brightness=35
