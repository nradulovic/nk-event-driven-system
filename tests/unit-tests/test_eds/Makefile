
BUILDDIR = builddir
SUBPROJECTS = subprojects
EXECUTABLE = $(BUILDDIR)/unit_testing

.PHONY: test
test: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: $(EXECUTABLE)
$(EXECUTABLE) : $(BUILDDIR)
	meson compile -C $(BUILDDIR)
	
.PHONY: clean
clean:
	meson subprojects purge --confirm
	rm -rf $(BUILDDIR)
	
.PHONY: dist-clean
dist-clean: clean
	rm -rf $(SUBPROJECTS)

$(SUBPROJECTS):
	mkdir $(SUBPROJECTS)
	meson wrap install gtest

$(BUILDDIR): $(SUBPROJECTS)
	meson setup $@
	