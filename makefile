.PHONY: build clean help run

help:
	@echo ""
	@echo "AMC: Petri Net Model Checker"
	@echo "================================================================================"

	@echo ""
	@echo "build:"
	@echo "   Build executable and all its dependencies"
	@echo ""
	@echo "   + BUILD_TYPE=[Release, Debug, RelWithDebInfo, MinSizeRel] (default: Release)"
	@echo "   | Type of build, i.e. the compiler settings to use."

	@echo ""
	@echo "clean:"
	@echo "   Remove all build artifacts and the 'out/' folder."

	@echo ""
	@echo "help:"
	@echo "   Prints this hopefully helpful piece of text."

	@echo ""
	@echo "run:"
	@echo "   Run the 'main' executable with its arguments"
	@echo ""
	@echo "   + i=<path to .pnml file> (default: pnml/mini_a.pnml)"
	@echo "   | Input file to run model checker on."
	@echo ""
	@echo "   + M=<MiB> (default: 1024)"
	@echo "   | Amount of memory (in MiB) to use."

BUILD_TYPE := Release

build:
	@mkdir -p build/
	@cd build && cmake -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) ..
	@cd build && $(MAKE) ${MAKE_FLAGS} main

clean:
	@rm -rf build/

M=1024
i=pnml/cycle_4.pnml

run:
	@./build/src/main -M $(M) -i $(i)
