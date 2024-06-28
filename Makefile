.PHONY: test clean assemble_tests

# =================================================
# Logic to select the right files and build options
# =================================================

ifndef BUILD_ENV_IS_DEFINED
$(error 'You need to use `source envfile` or something similar to	   \
	import the environment variables' )
endif

# compiles a list of object files based on source files.
# also preserves directory structure.
TEST_O=$(shell for i in $$(find "$(SDIR)" -name "*.c" ! -path 			\
	"$(TDIR)/*" ! -path "$(BENCHDIR)/*" ! -path "*$(IGNFLS)/*" 		\
	-type f); do [ $$i = "$(ENTRY)" ] && continue; base=$${i#$(SDIR)};	\
	name=$${base%.c}; pth=$${base%/*}; mkdir -p "$(BDIR)$$pth"; echo	\
	"$(BDIR)$$name.o"; done;)

# add main back in. Theoretically, with some changes, you could have multiple targets that use different objects.
APP_O=$(TEST_O) $(ENTRYOBJ)

ifeq '$(DEBUG)' 'true'
	CFLAGS=$(debug_flags) $(base_flags)
	LFLAGS=$(debug_lflags) $(base_lflags)
else
	CFLAGS=$(build_flags) $(base_flags)
	LFLAGS=$(build_lflags) $(base_lflags)
endif

# ==============================================
# This is where all of the actual make rules are
# ==============================================

$(APPNAME): $(APP_O)
	$(CC) $^ -o $@ $(LFLAGS)

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM -r $(TODIR)
	$(RM) -r $(BDIR)
	$(RM) app
	$(RM) -r test_logs
	$(RM) -r failed_tests
	mkdir -p $(BDIR)

# =============================================
# Run the python testing utility and copy files 
# =============================================

assemble_tests:
	@mkdir -p $(TODIR)
	@# pytest is being disabled for now
	@#./pytest.py
	@for i in $$(find '$(TDIR)/' ! -path "*/$(IGNFLS)/*"				\
				-name *.c -type f); do									\
		base=$${i#$(TDIR)};												\
		dirs=$${base%/*};												\
		mkdir -p $(TODIR)$$dirs;										\
		cp $(TDIR)$$base $(TODIR)$$base;								\
	done;

# ============================================
# Just a neat little automated testing utility
# ============================================

# The testing utility will dump results to stdout and to a directory of files with names of your choosing.
# Also, to skip tests, just put them in an __ignore__ dir (or whatever name you specified)

# Put the testing utility in a separate file
test: assemble_tests $(TEST_O)
	@env COMPLETE="$(COMPLETE)" TEST_O="$(TEST_O)" \
		TEST_FILES="$(TEST_FILES)" PRINT_SUCCESS="$(PRINT_SUCCESS)" \
		./testing.sh

# Also new benching utility
bench: assemble_tests $(TEST_O)
	@env TEST_O="$(TEST_O)" BENCH_TEST="$(BENCH_TEST)" \
		COMPLETE="$(COMPLETE)" ./benchmarking.sh

