TESTS = \
    test_cpy \
    test_ref testMpool

CFLAGS = -Wall -Werror -g
CH_FILE := test_ref.c test_cpy.c testMpool.c mpool.c mpool.h tst.c tst.h

# Control the build verbosity                                                   
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

GIT_HOOKS := .git/hooks/applied

.PHONY: all clean

all: $(GIT_HOOKS) $(TESTS)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

OBJS_LIB = \
    tst.o

OBJS := \
    $(OBJS_LIB) \
    test_cpy.o \
    test_ref.o testMpool.o

deps := $(OBJS:%.o=.%.o.d)

test_%: test_%.o $(OBJS_LIB)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^

testMpool: testMpool.o $(OBJS_LIB) mpool.o
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^


%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

clean:
	$(RM) $(TESTS) $(OBJS)
	$(RM) $(deps)

astyle:
	astyle --style=kr --indent=spaces=4 --suffix=none $(CH_FILE) 

TEST_DATA = s Tai 

cache-test_cpy: $(TESTS)    
	echo 3 | sudo tee /proc/sys/vm/drop_caches;
	perf stat --repeat 100 -e cache-misses,cache-references,instructions,cycles ./test_cpy --bench $(TEST_DATA)
cache-test_ref: $(TESTS)    
	echo 3 | sudo tee /proc/sys/vm/drop_caches;
	perf stat --repeat 100 -e cache-misses,cache-references,instructions,cycles ./test_ref --bench $(TEST_DATA)

cache-test_mpool: $(TESTS)    
	echo 3 | sudo tee /proc/sys/vm/drop_caches;
	sudo sh -c 'echo 1 >/proc/sys/kernel/perf_event_paranoid'
	perf stat --repeat 100 -e cache-misses,cache-references,instructions,cycles ./testMpool --bench $(TEST_DATA)

PERF = perf stat -o 
PERF_CMD = --repeat 100 -e cache-misses,cache-references,instructions,cycles 
cache-test: $(TESTS)    
	echo 3 | sudo tee /proc/sys/vm/drop_caches;
	sudo sh -c 'echo 1 >/proc/sys/kernel/perf_event_paranoid'
	$(PERF) test_cpy.txt $(PERF_CMD)  ./test_cpy --bench $(TEST_DATA)
	$(PERF) test_ref.txt $(PERF_CMD)  ./test_ref --bench $(TEST_DATA)
	$(PERF) test_mpool.txt $(PERF_CMD)  ./testMpool --bench $(TEST_DATA)

-include $(deps)
