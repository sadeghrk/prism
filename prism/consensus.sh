for k in 8 12 16 20  ; do					
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -gs -maxiters 2500000 > log/Coin/coin_4_$k\_s_gs.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_s_mpi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_s_topological_mpi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 3505000 > log/Coin/coin_4_$k\_ex_ii.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -explicitmodpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_s_topological_exmpi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -improvedmodpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_s_topological_tpmpi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -improvedmodpoliter2 -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_s_topological_spmpi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_ex_gs.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -politer -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_ex_pi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_4_$k\_ex_mpi.log
./bin/prism ../prism-examples/consensus/coin4.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -javamaxmem 75000m -topological -maxiters 2500000 > log/Coin/coin_4_$k\_ex_topological_gs.log
done

for k in 5 10 15; do
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -gs -maxiters 2500000 > log/Coin/coin_5_$k\_s_gs.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_s_mpi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_s_topological_mpi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 3505000 > log/Coin/coin_5_$k\_ex_ii.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -explicitmodpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_s_topological_exmpi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -improvedmodpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_s_topological_spmpi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -improvedmodpoliter2 -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_s_topological_tpmpi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_ex_gs.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -politer -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_ex_pi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_5_$k\_ex_mpi.log
./bin/prism ../prism-examples/consensus/coin5.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -javamaxmem 75000m -topological -maxiters 2500000 > log/Coin/coin_5_$k\_ex_topological_gs.log
done

for k in 5; do
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -gs -maxiters 2500000 > log/Coin/coin_6_$k\_s_gs.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_s_mpi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_s_topological_mpi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 3505000 > log/Coin/coin_6_$k\_ex_ii.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -explicitmodpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_s_topological_exmpi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -improvedmodpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_s_topological_spmpi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -s -topological -improvedmodpoliter2 -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_s_topological_tpmpi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_ex_gs.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -politer -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_ex_pi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -modpoliter -javamaxmem 75000m -maxiters 2500000 > log/Coin/coin_6_$k\_ex_mpi.log
./bin/prism ../prism-examples/consensus/coin6.nm ../prism-examples/consensus/coin.pctl -prop 4 -const K=$k -ex -gs -javamaxmem 75000m -topological -maxiters 2500000 > log/Coin/coin_6_$k\_ex_topological_gs.log
done