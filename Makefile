CONDOR = condor_povray
LOCAL = local_povray

FFMPEG = ./ffmpeg
POVRAY = ./povray

INPUTS = rubiks1080p.pov rubiks720p.pov rubiks480p.pov WRC_RubiksCube.inc
MOVIES = rubiks1080p.mp4 rubiks720p.mp4 rubiks480p.mp4 rubiks1080plocal.mp4
ERRORS = rubiks1080p.err rubiks720p.err rubiks480p.err rubiks1080plocal.err
LOGS = rubiks1080p.log rubiks720p.log rubiks480p.log rubiks1080plocal.log

CLEANUP = *.mpg *.log *.out *.png *.sub *.sh *.err *.mp4 rubiks1080p_outputs rubiks720p_outputs rubiks480p_outputs

$(FFMPEG): /bin/tar /usr/bin/wget
	wget https://www.johnvansickle.com/ffmpeg/old-releases/ffmpeg-3.3.4-64bit-static.tar.xz
	tar -xvf ffmpeg-3.3.4-64bit-static.tar.xz
	mv ffmpeg-3.3.4-64bit-static.tar.xz ffmpeg-3.3.4-64bit-static
	mv ffmpeg-3.3.4-64bit-static/ffmpeg ffmpeg
	rm -rf ffmpeg-3.3.4-64bit-static

$(POVRAY): /bin/gunzip /usr/bin/wget
	wget https://news.povray.org/povray.binaries.misc/attachment/%3Cweb.52fe465436b02b07d19b0ec40%40news.povray.org%3E/povray.gz
	gunzip povray.gz
	chmod 755 povray

rubiks1080p.mpg: $(CONDOR) $(FFMPEG) $(POVRAY) rubiks1080p.pov WRC_RubiksCube.inc
	./$(CONDOR) rubiks1080p.pov rubiks1080p.mp4 600 1920 1080

rubiks720p.mpg: $(CONDOR) $(FFMPEG) $(POVRAY) rubiks720p.pov WRC_RubiksCube.inc
	./$(CONDOR) rubiks720p.pov rubiks720p.mp4 600 1280 720

rubiks480p.mpg: $(CONDOR) $(POVRAY) rubiks480p.pov WRC_RubiksCube.inc
	./$(CONDOR) rubiks480p.pov rubiks480p.mp4 600 640 480

rubiks1080plocal.mpg: $(LOCAL) $(FFMPEG) $(POVRAY) rubiks1080p.pov WRC_RubiksCube.inc
	./$(LOCAL) rubiks1080p.pov rubiks1080plocal.mp4 600 1920 1080

rubiks720plocal.mpg: $(LOCAL) $(FFMPEG) $(POVRAY) rubiks720p.pov WRC_RubiksCube.inc
	./$(LOCAL) rubiks720p.pov rubiks480plocal.mp4 600 1280 720

rubiks480plocal.mpg: $(LOCAL) $(FFMPEG) $(POVRAY) rubiks480p.pov WRC_RubiksCube.inc
	./$(LOCAL) rubiks480p.pov rubiks480plocal.mp4 600 640 480

1080p: rubiks1080p.mp4

720p: rubiks720p.mp4

480p: rubiks480p.mp4

1080plocal: rubiks1080plocal.mp4

480plocal: rubiks480plocal.mp4

all: $(FFMPEG) $(POVRAY) $(MOVIES)

build: $(FFMPEG) $(POVRAY)

clean:
	condor_rm $(USER) || true
	rm -rf $(CLEANUP) $(FFMPEG) $(POVRAY) || true

lean:
	condor_rm $(USER) || true
	rm -rf $(CLEANUP) || true
