class TwitchHelper
  require 'redis'
  require 'redis-queue'
  def self.parseReceived(redis, m)
    case m.message
    when /^!left$/
      self.send(redis, m, :l)
    when /^!right$/
      self.send(redis, m, :r)
    when /^!forward$/
      self.send(redis, m, :f)
    when /^!back$/
      self.send(redis, m, :b)
    when /^!kick$/
      self.send(redis, m, :k)
    when /^!nop$/
      self.send(redis, m, :n)
    else
      5.times { puts }
      puts "#{m.channel} ECE477: Invalid Command"
      5.times { puts }
    end
  end

  def self.send(redis, m, command)
    bot = self.bot(m.channel)
    self.print(bot.to_s + " | " + m.user.nick + ": " + command.to_s)
    queue = Redis::Queue.new(bot.to_s, 'steelmesh', :redis => redis)
    queue << command.to_json
  end

  def self.bot(channel)
    if channel == "#ece477group1" then :bot1 else :bot2 end
  end

  def self.print(m)
    10.times { puts }
    puts m
    10.times { puts }
  end
end
