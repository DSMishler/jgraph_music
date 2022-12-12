jgrs = $(wildcard ./*.jgr)
JGRAPH_LOCAL = ../../jgraph/jgraph
JGRAPH_HYDRA = /home/jplank/bin/LINUX-X86_64/jgraph
JGRAPH=$(JGRAPH_HYDRA)


files:
	g++ -o mjm_to_jgr -Wall -pedantic mjm_to_jgr.cpp
	$(MAKE) -C symbols

clean:
	rm mjm_to_jgr

junk:
	$(JGRAPH) -P $@.jgr | ps2pdf - | convert -density 300 - -quality 100 $@.jpg

testjgr:
	$(JGRAPH) -P $@.jgr | ps2pdf - | convert -density 300 - -quality 100 $@.jpg

echo:
	echo $(jgrs)
	echo $@

examples: files maryhad moonlight america buns grace

maryhad:
	./mjm_to_jgr < ./mjms/mary_had_a_little_lamb.mjm > ./jgrs/$@.jgr
	$(JGRAPH) -P ./jgrs/$@.jgr | ps2pdf - | convert -density 300 - -quality 100 ./music/$@.jpg

moonlight:
	./mjm_to_jgr < ./mjms/moonlight_sonata.mjm > ./jgrs/$@.jgr
	$(JGRAPH) -P ./jgrs/$@.jgr | ps2pdf - | convert -density 300 - -quality 100 ./music/$@.jpg

america:
	./mjm_to_jgr < ./mjms/america.mjm > ./jgrs/$@.jgr
	$(JGRAPH) -P ./jgrs/$@.jgr | ps2pdf - | convert -density 300 - -quality 100 ./music/$@.jpg

buns:
	./mjm_to_jgr < ./mjms/hot_cross_buns.mjm > ./jgrs/$@.jgr
	$(JGRAPH) -P ./jgrs/$@.jgr | ps2pdf - | convert -density 300 - -quality 100 ./music/$@.jpg

grace:
	./mjm_to_jgr < ./mjms/amazing_grace.mjm > ./jgrs/$@.jgr
	$(JGRAPH) -P ./jgrs/$@.jgr | ps2pdf - | convert -density 300 - -quality 100 ./music/$@.jpg
