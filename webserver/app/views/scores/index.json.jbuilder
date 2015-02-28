json.array!(@scores) do |score|
  json.extract! score, :id, :team, :points
  json.url score_url(score, format: :json)
end
