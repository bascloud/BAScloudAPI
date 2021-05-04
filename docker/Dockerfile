FROM python:3.8

VOLUME /notebook/persistent
WORKDIR /notebook
ENV LD_LIBRARY_PATH "/usr/local/lib/python3.8/site-packages:$LD_LIBRARY_PATH"

#RUN apt update \
#    && apt install libstdc++6
RUN pip install --no-cache-dir jupyter

ADD https://github.com/bascloud/BAScloudAPI/releases/download/v0.1.0/pyBAScloudAPI-0.1.0-cp38-cp38-linux_x86_64.whl /tmp/pyBAScloudAPI-0.1.0-cp38-cp38-linux_x86_64.whl
RUN pip install --no-cache-dir /tmp/pyBAScloudAPI-0.1.0-cp38-cp38-linux_x86_64.whl \
    && rm /tmp/pyBAScloudAPI-0.1.0-cp38-cp38-linux_x86_64.whl

COPY ./docker/jupyter_notebook_config.py /notebook/jupyter_notebook_config.py
COPY ./pyBAScloudAPI/examples /notebook/examples

CMD [ "jupyter", "notebook", "--allow-root", "/notebook" ]