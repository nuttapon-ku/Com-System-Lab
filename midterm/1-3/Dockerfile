FROM python:2

ADD insertionwrapper.py data1d-100000 insertion.so /app/

RUN pip install pystrich

CMD [ "python", "./app/pywrapper.py" ]