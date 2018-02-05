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

for k in 1 2 3 4; do
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -ex -gs -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_ex_gs.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -ex -politer -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_ex_pi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -ex -modpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_ex_mpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -ex -gs -javamaxmem 75000m -topological -maxiters 500000 > log/D/dns$k\_ex_topological_gs.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -s -gs -maxiters 500000 > log/D/dns$k\_s_gs.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -s -modpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_mpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 3505000 > log/D/dns$k\_ex_ii.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -s -explicitmodpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_exmpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -s -improvedmodpoliter -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_spmpi.log
./bin/prism ../prism-examples/nonsparse/dns$k.nm ../prism-examples/nonsparse/qq.pctl -s -improvedmodpoliter2 -javamaxmem 75000m -maxiters 500000 > log/D/dns$k\_s_tpmpi.log
done

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

for k in 17 19; do
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -pow -nocompact > log/Herman/h_$k\_50_s_pow.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -pgs -nocompact > log/Herman/h_$k\_50_s_pgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -sor -nocompact > log/Herman/h_$k\_50_s_sor.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -jac -nocompact > log/Herman/h_$k\_50_s_j.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -gs -nocompact > log/Herman/h_$k\_50_s_gs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -improvedgs -nocompact > log/Herman/h_$k\_50_s_spgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -improvedgs2 -nocompact > log/Herman/h_$k\_50_s_tpgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -ex -prop 6 -topological -gs -javamaxmem 90000m > log/Herman/h_$k\_50_ex_topological_gs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -gs -ii -epsilon 1e-8 -javamaxmem 90000m > log/Herman/h_$k\_50_s_ii.log

./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -pow -nocompact > log/Herman/h_$k\_75_pow_j.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -pgs -nocompact > log/Herman/h_$k\_75_pgs_j.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -sor -nocompact > log/Herman/h_$k\_75_sor_j.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -jac -nocompact > log/Herman/h_$k\_75_s_j.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -gs -nocompact > log/Herman/h_$k\_75_s_gs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -improvedgs -nocompact > log/Herman/h_$k\_75_s_spgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -improvedgs2 -nocompact > log/Herman/h_$k\_75_s_tpgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -ex -prop 6 -gs -topological -javamaxmem 90000m > log/Herman/h_$k\_75_ex_gs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_75.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -gs -ii -epsilon 1e-8 -javamaxmem 90000m > log/Herman/h_$k\_75_s_ii.log

./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -pow -nocompact > log/Herman/h_$k\_9_s_pow.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -pgs -nocompact > log/Herman/h_$k\_9_s_pgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -sor -nocompact > log/Herman/h_$k\_9_s_sor.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -jac -nocompact > log/Herman/h_$k\_9_s_j.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -gs -nocompact > log/Herman/h_$k\_9_s_gs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -improvedgs -nocompact > log/Herman/h_$k\_9_s_spgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -improvedgs2 -nocompact > log/Herman/h_$k\_9_s_tpgs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -ex -prop 6 -gs -topological -javamaxmem 90000m > log/Herman/h_$k\_9_ex_gs.log
./bin/prism ../prism-examples/self-stabilisation/herman/herman$k\_9.pm ../prism-examples/self-stabilisation/herman/herman.pctl -s -prop 6 -gs -ii -epsilon 1e-8 -javamaxmem 90000m > log/Herman/h_$k\_9_s_ii.log
done