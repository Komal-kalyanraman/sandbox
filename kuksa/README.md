Git repo is from link: https://github.com/eclipse-kuksa/kuksa-databroker/tree/0.4.4

1. Go to folder kuksa/vss
2. Run below command:
   docker run -it --rm --net=host -v $(pwd):/data ghcr.io/eclipse-kuksa/kuksa-databroker:0.4.4 --metadata /data/custom_vss.json
