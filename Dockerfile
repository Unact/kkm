FROM ruby:2.4.2

RUN apt-get clean
RUN echo "deb [check-valid-until=no] http://archive.debian.org/debian jessie-backports main" > /etc/apt/sources.list.d/jessie-backports.list
RUN sed -i '/deb http:\/\/deb.debian.org\/debian jessie-updates main/d' /etc/apt/sources.list
RUN apt-get -o Acquire::Check-Valid-Until=false update
RUN apt-get install locales locales-all -y

RUN locale-gen ru_RU.UTF-8
ENV LANG ru_RU.UTF-8
ENV LANGUAGE ru_RU:ru
ENV LC_ALL ru_RU.UTF-8
RUN gem install bundler -v 1.17.1

ENV app /app

RUN mkdir $app
WORKDIR $app
ADD . $app

RUN bundle install

CMD ["bash"]
