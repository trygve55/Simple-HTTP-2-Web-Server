from hyper import HTTPConnection
c = HTTPConnection('localhost:8080')
c.request('GET', '/')
print(c.get_response())
