FROM ruby:2.1.10

RUN apt-get clean && apt-get update
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
