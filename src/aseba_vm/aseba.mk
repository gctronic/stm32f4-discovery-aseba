ASEBA = ./aseba

ASEBASRC = $(ASEBA)/transport/buffer/vm-buffer.c \
           $(ASEBA)/vm/natives.c \
           $(ASEBA)/vm/vm.c \
           $(ASEBA)/transport/can/can-net.c \
           $(ASEBA)/transport/can/can-buffer.c \
           src/aseba_vm/aseba_can_interface.c \
           src/aseba_vm/aseba_node.c \
           src/aseba_vm/skel.c

ASEBAINC = $(ASEBA) \
           src
