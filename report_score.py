import urllib2
import urllib
query_args = { 'Team':'2' }
url = 'http://192.168.1.7:3000/score'
data = urllib.urlencode(query_args)
request = urllib2.Request(url, data)
response = urllib2.urlopen(request).read()
print response
