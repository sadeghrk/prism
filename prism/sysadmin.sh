for k in 1 2 3 4; do
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -ex -gs -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_ex_gs.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -ex -politer -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_ex_pi.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -ex -modpoliter -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_ex_mpi.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -ex -gs -javamaxmem 75000m -topological -maxiters 500000 > log/SysAdmin/SysAdmin$k\_ex_topological_gs.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -s -gs -maxiters 500000 > log/SysAdmin/SysAdmin$k\_s_gs.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -s -modpoliter -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_s_mpi.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 3505000 > log/SysAdmin/SysAdmin$k\_ex_ii.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -s -explicitmodpoliter -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_s_exmpi.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -s -improvedmodpoliter -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_s_spmpi.log
./bin/prism ../prism-examples/nonsparse/SysAdmin$k.nm ../prism-examples/nonsparse/qq.pctl -s -improvedmodpoliter2 -javamaxmem 75000m -maxiters 500000 > log/SysAdmin/SysAdmin$k\_s_tpmpi.log
done

