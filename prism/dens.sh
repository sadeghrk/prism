for k in `seq 1 200`; do
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -ex -gs -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_ex_gs.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -ex -politer -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_ex_pi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -ex -modpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_ex_mpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -ex -gs -javamaxmem 75000m -topological -maxiters 500000 > log/D/dns$k\_ex_topological_gs.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -s -gs -maxiters 500000 > log/D/dns$k\_s_gs.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -s -modpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_mpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 3505000 > log/D/dns$k\_ex_ii.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -s -explicitmodpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_exmpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -s -improvedmodpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_spmpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/dns.pctl -s -improvedmodpoliter2 -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_tpmpi.log
done

