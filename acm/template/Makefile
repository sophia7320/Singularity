CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

TARGETS = qpow combinations gcd_exgcd prime_sieve bit segtree dsu string_hash kmp dijkstra topo st_table trie

.PHONY: all clean test

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: all
	@echo "=== Running all tests ==="
	@passed=0; failed=0; \
	for t in $(TARGETS); do \
		echo -n "$$t: "; \
		if ./$$t 2>&1; then \
			passed=$$((passed+1)); \
		else \
			failed=$$((failed+1)); \
		fi; \
	done; \
	echo "=== $$passed passed, $$failed failed ==="; \
	[ $$failed -eq 0 ]

clean:
	rm -f $(TARGETS)
