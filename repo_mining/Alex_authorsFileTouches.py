import json
import requests
import csv
import os

if not os.path.exists("data"):
    os.makedirs("data")

# GitHub Authentication function
def github_auth(url, lsttoken, ct):
    jsonData = None
    try:
        ct = ct % len(lstTokens)
        headers = {'Authorization': 'Bearer {}'.format(lsttoken[ct])}
        request = requests.get(url, headers=headers)
        jsonData = json.loads(request.content)
        ct += 1
    except Exception as e:
        pass
        print(e)
    return jsonData, ct

# Function to get authors and dates for each file
def get_authors_and_dates(dictauthors, lsttokens, repo, filename):
    ct = 0  # token counter

    try:
        commitsUrl = f'https://api.github.com/repos/{repo}/commits?path={filename}&per_page=100'
        jsonCommits, ct = github_auth(commitsUrl, lsttokens, ct)

        for commit in jsonCommits:
            author_name = commit['commit']['author']['name']
            commit_date = commit['commit']['author']['date']
            dictauthors.setdefault(filename, []).append({'author': author_name, 'date': commit_date})
    except Exception as e:
        print(f"Error retrieving authors and dates for {filename}: {e}")

# Function to count files
def count_files(dictfiles, dictauthors, lsttokens, repo):
    ipage = 1  # url page counter
    ct = 0  # token counter

    try:
        # loop though all the commit pages until the last returned empty page
        while True:
            spage = str(ipage)
            commitsUrl = 'https://api.github.com/repos/' + repo + '/commits?page=' + spage + '&per_page=100'
            jsonCommits, ct = github_auth(commitsUrl, lsttokens, ct)

            # break out of the while loop if there are no more commits in the pages
            if len(jsonCommits) == 0:
                break
            # iterate through the list of commits in  spage
            for shaObject in jsonCommits:
                sha = shaObject['sha']
                # For each commit, use the GitHub commit API to extract the files touched by the commit
                shaUrl = 'https://api.github.com/repos/' + repo + '/commits/' + sha
                shaDetails, ct = github_auth(shaUrl, lsttokens, ct)
                filesjson = shaDetails['files']
                for filenameObj in filesjson:
                    filename = filenameObj['filename']
                    dictfiles[filename] = dictfiles.get(filename, 0) + 1
                    print(filename)
                    get_authors_and_dates(dictauthors, lsttokens, repo, filename)
            ipage += 1
    except Exception as e:
        print(f"Error counting files: {e}")
        exit(0)

# GitHub repo
repo = 'scottyab/rootbeer'
# repo = 'Skyscanner/backpack' # This repo is commit heavy. It takes long to finish executing
# repo = 'k9mail/k-9' # This repo is commit heavy. It takes long to finish executing
# repo = 'mendhak/gpslogger'

# put your tokens here
# Remember to empty the list when going to commit to GitHub.
# Otherwise, they will all be reverted and you will have to re-create them
# I would advise creating more than one token for repos with heavy commits
lstTokens = ["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"]

dictFiles = dict()
dictAuthors = dict()
count_files(dictFiles, dictAuthors, lstTokens, repo)

print('Total number of files: ' + str(len(dictFiles)))

file = repo.split('/')[1]
# change this to the path of your file
fileOutput = 'data/file_' + file + '.csv'
fileAuthorsOutput = 'data/authors_' + file + '.csv'

# Writing file touches to CSV
rows = ["Filename", "Touches"]
fileCSV = open(fileOutput, 'w')
writer = csv.writer(fileCSV)
writer.writerow(rows)

bigcount = None
bigfilename = None
for filename, count in dictFiles.items():
    rows = [filename, count]
    writer.writerow(rows)
    if bigcount is None or count > bigcount:
        bigcount = count
        bigfilename = filename
fileCSV.close()
print('The file ' + bigfilename + ' has been touched ' + str(bigcount) + ' times.')

# Writing authors and dates to CSV
rows = ["Filename", "Author", "Date"]
fileAuthorsCSV = open(fileAuthorsOutput, 'w')
writerAuthors = csv.writer(fileAuthorsCSV)
writerAuthors.writerow(rows)

for filename, authors_and_dates in dictAuthors.items():
    for entry in authors_and_dates:
        rows = [filename, entry['author'], entry['date']]
        writerAuthors.writerow(rows)
fileAuthorsCSV.close()

