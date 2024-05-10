Git repo is from link: https://github.com/eclipse-kuksa/kuksa-databroker/tree/0.4.4

Quickstart link for python example: https://github.com/eclipse/kuksa.val/blob/master/doc/quickstart.md

1. Go to folder kuksa/vss
2. Run command: docker run -it --rm --net=host -v $(pwd):/data ghcr.io/eclipse-kuksa/kuksa-databroker:0.4.4 --metadata /data/custom_vss.json
