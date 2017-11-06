# Parent image
FROM ubuntu:16.04

# Install any needed packages specified in requirements.txt
RUN apt-get update && apt-get install -y \
  cmake \
  git \
  gcc \
  pkg-config libssl-dev libsasl2-dev \
  automake autoconf libtool \
  python \
  python-pip

WORKDIR /tmp

# Libmongoc
RUN git clone https://github.com/mongodb/mongo-c-driver.git

WORKDIR /tmp/mongo-c-driver

RUN git checkout master
RUN ./autogen.sh --with-libbson=bundled
RUN make
RUN make install


WORKDIR /tmp

# Libmongocxx
RUN git clone https://github.com/mongodb/mongo-cxx-driver.git

WORKDIR /tmp/mongo-cxx-driver/build

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
RUN make EP_mnmlstc_core
RUN make && make install