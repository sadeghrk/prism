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

