FROM python:3.10

RUN apt update && apt upgrade -y

RUN apt install build-essential -y
RUN mkdir /code
WORKDIR /code
COPY ./ /code
RUN pip install -r requirements.txt

CMD python3 graph.py {args.num_peers} > graph && make && ./blockSim
