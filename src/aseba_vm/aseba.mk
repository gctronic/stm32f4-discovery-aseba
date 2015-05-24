ASEBA = ./aseba

ASEBASRC = $(ASEBA)/transport/buffer/vm-buffer.c \
           $(ASEBA)/vm/natives.c \
           $(ASEBA)/vm/vm.c \
           src/aseba_vm/aseba_can_interface.c \
           src/aseba_vm/aseba_node.c \
           src/aseba_vm/skel.c \
           src/serial-can-bridge/serial_can_bridge.c

ASEBAINC = $(ASEBA) \
           src
