
# Add library include folder
CC_INCLUDES += ../event-driven-system/source

AR_LIBS += $(WS)/generated/event-driven-system.a

$(WS)/generated/event-driven-system.a:
	$(MAKE) -C . -f library.Makefile
