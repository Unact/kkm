FROM ruby:2.1.10

COPY Gemfile /
RUN apt-get clean && apt-get update
RUN apt-get install locales locales-all -y
RUN locale-gen ru_RU.UTF-8
ENV LANG ru_RU.UTF-8
ENV LANGUAGE ru_RU:ru
ENV LC_ALL ru_RU.UTF-8
RUN gem install bundler
RUN bundle install --standalone --clean --no-deployment

CMD ["bash"]
