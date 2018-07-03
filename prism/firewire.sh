for dl in 6 21; do
for dd in 3500; do

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -ex -const delay=$dl,deadline=$dd,fast=.5 -gs -javamaxmem 2400m> log/Firewire/firewire_delay_$dl\_deadline_$dd\_ex_gs.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -ex -const delay=$dl,deadline=$dd,fast=.5 -modpoliter -javamaxmem 2400m > log/Firewire/firewire_delay_$dl\_deadline_$dd\_ex_mpi.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -ex -const delay=$dl,deadline=$dd,fast=.5 -gs -topological -javamaxmem 2400m > log/Firewire/firewire_delay_$dl\_deadline_$dd\_ex_topological_gs.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -ex -const delay=$dl,deadline=$dd,fast=.5 -gs -ii -javamaxmem 2400m > log/Firewire/firewire_delay_$dl\_deadline_$dd\_ex_ii.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -gs > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_gs.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -modpoliter > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_mpi.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -explicitmodpoliter > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_explicit_mpi.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -improvedmodpoliter > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_improved_mpi.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -improvedmodpoliter2 > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_improved2_mpi.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -bucketvaliter > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_bucket_vi.log

./bin/prism ../prism-examples/firewire/abst/deadline.nm ../prism-examples/firewire/abst/deadline.pctl -s -const delay=$dl,deadline=$dd,fast=.5 -backwardvaliter > log/Firewire/firewire_delay_$dl\_deadline_$dd\_s_backward_vi.log

done
done	
