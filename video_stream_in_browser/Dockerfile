FROM python:3.8-slim-buster

WORKDIR /app

COPY app/requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY app .

CMD [ "python", "-m" , "flask", "run", "--host=0.0.0.0"]
