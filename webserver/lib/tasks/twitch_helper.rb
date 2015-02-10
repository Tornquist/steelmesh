class TwitchHelper
  require 'redis'
  def self.parseReceived(queue, m)
    case m.message
    when /^!left$/
      self.send(queue, m, :left)
    when /^!right$/
      self.send(queue, m, :right)
    when /^!forward$/
      self.send(queue, m, :forward)
    when /^!reverse$/
      self.send(queue, m, :reverse)
    when /^!kick$/
      self.send(queue, m, :kick)
    else
      5.times { puts }
      puts "#{m.channel} ECE477: Invalid Command"
      5.times { puts }
    end
  end

  def self.send(queue, m, command)
    bot = self.bot(m.channel)
    self.print(bot.to_s + " | " + m.user.nick + ": " + command.to_s)
    if (bot == :bot1)
      queue.publish("steelmesh:bot1", command.to_json)
    elsif (bot == :bot2)
      queue.publish("steelmesh:bot2", command.to_json)
    end
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
