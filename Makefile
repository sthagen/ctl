CC = gcc -std=c99
CXX = g++ -std=c++17

LONG = 0
SANITIZE = 0
SRAND = 1
COMPARE_A = 0

O0 = 0
O1 = 0
O2 = 0
O3 = 0
Og = 0
Ofast = 0

CFLAGS  = -Ictl
CFLAGS += -Wall -Wextra -Wpedantic -Wfatal-errors -Wshadow
CFLAGS += -march=native
CFLAGS += -g

ifeq (1, $(LONG))
CFLAGS += -Werror
CFLAGS += -DLONG
endif

ifeq (1, $(SANITIZE))
CFLAGS += -fsanitize=address -fsanitize=undefined
endif

ifeq (1, $(Og))
CFLAGS += -Og
endif

ifeq (1, $(O0))
CFLAGS += -O0
endif

ifeq (1, $(O1))
CFLAGS += -O1
endif

ifeq (1, $(O2))
CFLAGS += -O2
endif

ifeq (1, $(O3))
CFLAGS += -O3
endif

ifeq (1, $(Ofast))
CFLAGS += -Ofast
endif

ifeq (1, $(SRAND))
CFLAGS += -DSRAND
endif

ifeq (1, $(COMPARE_A))
CFLAGS += -DCOMPARE_A
endif

define expand
	@$(CC) $(CFLAGS) ctl/$(1).h -E $(2) | clang-format -style=webkit
endef

BINS = \
tests/test_c99 \
tests/test_container_composing \
tests/test_deq \
tests/test_lst \
tests/test_map \
tests/test_str \
tests/test_pqu \
tests/test_que \
tests/test_set \
tests/test_stk \
tests/test_vec_capacity \
tests/test_vec

BINS_PERF = \
tests/perf_vec_pop_back \
tests/perf_vec_push_back \
tests/perf_vec_sort \
tests/perf_vector_pop_back \
tests/perf_vector_push_back \
tests/perf_vector_sort \

all: $(BINS)
	$(foreach bin,$(BINS),./$(bin) &&) exit 0
	@$(CC) --version
	@$(CXX) --version

ALWAYS:

# PERF C
tests/perf_vec_pop_back:  		ALWAYS; $(CC) $(CFLAGS) -O3 $@.c -o $@
tests/perf_vec_push_back: 		ALWAYS; $(CC) $(CFLAGS) -O3 $@.c -o $@
tests/perf_vec_sort: 	  		ALWAYS; $(CC) $(CFLAGS) -O3 $@.c -o $@

# PERF C++
tests/perf_vector_pop_back:     ALWAYS; $(CXX) $(CFLAGS) -O3 $@.cc -o $@
tests/perf_vector_push_back:    ALWAYS; $(CXX) $(CFLAGS) -O3 $@.cc -o $@
tests/perf_vector_sort: 	    ALWAYS; $(CXX) $(CFLAGS) -O3 $@.cc -o $@

tests/test_c99: 				ALWAYS; $(CC) $(CFLAGS) $@.c -o $@
tests/test_container_composing: ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_deq: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_lst: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_map: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_pqu: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_que: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_set: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_stk: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_str: 				ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_vec_capacity: 		ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@
tests/test_vec: 		 		ALWAYS; $(CXX) $(CFLAGS) $@.cc -o $@

# PERFORMANCE.

perf_vec: \
tests/perf_vector_push_back \
tests/perf_vec_push_back \
tests/perf_vector_pop_back \
tests/perf_vec_pop_back \
tests/perf_vector_sort \
tests/perf_vec_sort
	./$(word 1,$^) >  $@.log
	./$(word 2,$^) >> $@.log
	./$(word 3,$^) >> $@.log
	./$(word 4,$^) >> $@.log
	./$(word 5,$^) >> $@.log
	./$(word 6,$^) >> $@.log
	python3 tests/perf_plot.py $@.log
	mv $@.log.png images/

perf: perf_vec

clean:
	@rm -f $(BINS)
	@rm -f $(BINS_PERF)
	@rm -f perf_*.log

# EXPANSIONS.
str:
	$(call expand,$@)

lst:
	$(call expand,$@,-DT=int -DP)

vec:
	$(call expand,$@,-DT=int -DP)

deq:
	$(call expand,$@,-DT=int -DP)

stk:
	$(call expand,$@,-DT=int -DP)

que:
	$(call expand,$@,-DT=int -DP)

pqu:
	$(call expand,$@,-DT=int -DP)

map:
	$(call expand,$@,-DT=int -DU=int -DP)

set:
	$(call expand,$@,-DT=int -DP)
