for K in 8 9 10; do

./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -gs -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_gs.log

#./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -modpoliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_mpi.log

#./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -explicitmodpoliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_expmpi.log

#./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -improvedmodpoliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_improved_mpi.log

#./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -improvedmodpoliter2 -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_improved_mpi2.log

#./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -bucketvaliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_bucket_vi.log

#./bin/prism ../prism-examples/wlan/wlan4_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=600,COL=$K,k=$K -s -backwardvaliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_4_k$K\_s_backward_vi.log

done

for K in 8 9 10; do

./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -gs -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_gs.log

./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -modpoliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_mpi.log

./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -explicitmodpoliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_expmpi.log

#./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -improvedmodpoliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_improved_mpi.log

#./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -improvedmodpoliter2 -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_improved_mpi2.log

#./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -bucketvaliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_bucket_vi.log

#./bin/prism ../prism-examples/wlan/wlan5_collide.nm ../prism-examples/wlan/wlan_collide.pctl -const TRANS_TIME_MAX=160,COL=$K,k=$K -s -backwardvaliter -javamaxmem 75000m -maxiters 500000 > log/Wlan/wlan_collide_5_k$K\_s_backward_vi.log

done
