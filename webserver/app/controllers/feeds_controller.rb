class FeedsController < ApplicationController
  require 'FileUtils'
  def start
    id = params["id"].to_i
    if id == 0
      render :text => "Invalid ID"
    else
      path = "feeds/temp_#{id}.txt"
      if File.exists?(path)
        FileUtils.rm(path)
      end
      File.open(path,'w') do |file|
      end
      render :text => "Cleaned Team #{id} temp file"
    end
  end

  def image
    id = params["id"].to_i
    if id == 0
      render :text => "Invalid ID"
    else
      path = "feeds/temp_#{id}.txt"
      File.open(path, 'ab') do |file|
        file.write(hex_to_bin(params["data"]))
      end
      render :text => "Appended Data to Team #{id}"
    end
  end

  def end
    id = params["id"].to_i
    if id == 0
      render :text => "Invalid ID"
    else
      src = "feeds/temp_#{id}.txt"
      dest = "feeds/#{id}.txt"
      if File.exists?(src)
        FileUtils.mv(src, dest)
      end
      render :text => "Launched Team #{id} Data"
    end
  end

  def hex_to_bin(s)
   s.scan(/../).map { |x| x.hex.chr }.join
  end
end
