FROM ubuntu:12.04
MAINTAINER Kevin Smyth <ksmyth@isis.vanderbilt.edu>

# create user
RUN adduser --disabled-password --home=/jenkins --gecos "" jenkins
WORKDIR /jenkins

RUN apt-get update && apt-get install -y fakeroot debhelper build-essential autoconf libtool libxerces-c2-dev autotools-dev ccache libcppunit-dev libxalan110-dev ant junit ant-optional pccts libboost-dev zlib1g-dev swig openjdk-7-jdk libboost-python-dev libjson-spirit-dev

RUN apt-get update && apt-get install -y curl wget unzip

RUN curl -o /jenkins/slave.jar -f -s -S http://build.isis.vanderbilt.edu/jnlpJars/slave.jar
ENV JAVA_HOME /usr/lib/jvm/java-7-openjdk-amd64/

USER jenkins
CMD ["java", "-jar", "slave.jar", "-jnlpUrl", "http://build.isis.vanderbilt.edu/computer/UDM_ubuntu12.04/slave-agent.jnlp"]
