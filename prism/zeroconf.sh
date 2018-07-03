for K in 14 18; do
for N in 50 400; do

#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -ex -gs -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_ex_gs.log
#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -ex -politer -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_ex_pi.log
#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -ex -modpoliter -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_ex_mpi.log
#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -ex -gs -topological -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_ex_topological_gs.log
#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -ex -gs -ii -epsilon 1e-8 -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_ex_ii.log

#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -gs -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_gs.log

./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -topological -modpoliter -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_mpi.log

#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -topological -explicitmodpoliter -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_expmpi.log

./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -topological -improvedmodpoliter -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_improved_mpi.log

./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -topological -improvedmodpoliter2 -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_improved_mpi2.log

#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -topological -bucketvaliter -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_bucket_vi.log

#./bin/prism ../prism-examples/zeroconf/zeroconf.nm ../prism-examples/zeroconf/zeroconf.pctl -prop 2 -const reset=false,err=.1,N=$N,K=$K -s -topological -backwardvaliter -javamaxmem 75000m -maxiters 500000 > log/zeroconf/zeroconf_K$K\_N$N\_s_backward_vi.log

done;
done;
