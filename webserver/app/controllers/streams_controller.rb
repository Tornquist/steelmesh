class StreamsController < ApplicationController
  include ActionController::Live

  def bot1
    response.headers['Content-Type'] = 'text/event-stream'
    redis = Redis.new
    redis.subscribe('steelmesh:bot1') do |on|
      on.message do |event, data|
        response.stream.write("bot1 :#{ data }\n\n")
      end
    end
  ensure
    response.stream.close
  end

  def bot2
    response.headers['Content-Type'] = 'text/event-stream'
    redis = Redis.new
    redis.subscribe('steelmesh:bot2') do |on|
      on.message do |event, data|
        response.stream.write("bot2 :#{ data }\n\n")
      end
    end
  ensure
    response.stream.close
  end
end

