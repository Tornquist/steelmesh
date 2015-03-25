class CommandsController < ApplicationController
  def get
    @command = "No Command"
    id = params[:id]
    redis = Redis.new
    queue = Redis::Queue.new("bot#{id}", 'steelmesh', :redis => redis)
    if !queue.empty?
      @command = queue.pop
    end
    render json: @command
  end
end
