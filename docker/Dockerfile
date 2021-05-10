### Base
FROM python:alpine AS base

# Add notebook user (and group) and prepare persistent volume
RUN adduser -D -u 200000 -g notebook -h /notebook/ notebook
RUN mkdir -p /notebook/persistent && chown notebook:notebook /notebook/persistent
VOLUME /notebook/persistent

WORKDIR /notebook
ENV LD_LIBRARY_PATH "/usr/local/lib/python3.9/site-packages:$LD_LIBRARY_PATH"

CMD [ "jupyter", "notebook", "--ip=*", "--no-browser" ]

# Build dependencies for jupyter
RUN apk update --no-cache \
    && apk add --no-cache build-base python3-dev zeromq-dev libffi-dev py3-matplotlib
RUN python -m pip install --no-cache-dir jupyter


### Builder
FROM base AS builder
WORKDIR /tmp/build

RUN apk add --no-cache openssl-dev git cmake ninja
RUN python -m pip install build

COPY . /tmp/build/
RUN python setup.py bdist_wheel


### Result
FROM base

COPY --from=builder /tmp/build/dist/pyBAScloudAPI-*.whl /tmp/
RUN python -m pip install /tmp/pyBAScloudAPI-*.whl \
    && rm -r /tmp/pyBAScloudAPI-*.whl

COPY --chown=notebook:notebook ./pyBAScloudAPI/examples /notebook/examples

USER notebook
