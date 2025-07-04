#!/bin/bash

#note: to filter further --> uncomment any or none of the if statements below
#note: can add more filters below to speed it up
    #ie: test to see if a subscription has any sql server groups at all instead of testing in each resource of that subscription

#compiling example: chmod +x utility-test-all-subscriptions-on-specific-rule.sh && chmod +x cis-10.1-ensure-auditing-set-on-for-azure-sql-servers.sh
#running example without csv output: ./utility-test-all-subscriptions-on-specific-rule.sh -n ./cis-10.1-ensure-auditing-set-on-for-azure-sql-servers.sh
    #running example with csv output: ./utility-test-all-subscriptions-on-specific-rule.sh -n ./cis-10.1-ensure-auditing-set-on-for-azure-sql-servers.sh -c

#WARNING: run 8.1 if want to run 8.1-8.4 as 8.2-8.4 already does the same thing as 8.1

declare p_NAME_Of_SCRIPT=""; #had to use this and not the common-menu because it didn't work for some reason(ie: only write everything in one file)
declare p_CSV="False";

while getopts "n:c" option
do 
    case "${option}" in
        n)
            # Set name of the script
            p_NAME_Of_SCRIPT="${OPTARG}";;
        c)
            # Set csv flag to true
            p_CSV="True";;
    esac;
done;

script_name=$p_NAME_Of_SCRIPT

if [[ -z "$script_name" ]]; then
  echo "❌ script_name is not set. Exiting."
  exit 1
fi

log_dir="./logs"
mkdir -p "$log_dir"

# Get all subscription IDs
subscriptions=$(az account list --query "[].id" -o tsv) #240 subscriptions, with --all it's 271
# subscriptions="sub-id-1 sub-id-2 sub-id-3 sub-id-4 sub-id-5" #this is just an example if want to use a hard coded list

if [[ -z "$subscriptions" ]]; then
    echo "❌ No subscriptions found. Make sure you're logged in with 'az login'."
    exit 1
fi

echo "🔁 Running tests across all subscriptions and resource groups..."
echo "Logging output to $log_dir"

subscriptions_array=($subscriptions)
total_subs=${#subscriptions_array[@]}
current=1

for subscription in "${subscriptions_array[@]}"; do
    echo "🔷 [$current/$total_subs] Switching to subscription: $subscription"
    ((current++))

    az account set --subscription "$subscription"

    #commment me out if want to include visual studio subscription ids & names
    subscription_json_list=$(az account subscription list --query "[?subscriptionId=='$subscription']" --output="json" 2>/dev/null)
    subscription_display_name=$(jq -rc '.[0].displayName // ""' <<< "$subscription_json_list")
    if [[ $subscription_display_name == "Visual Studio"* ]]; then
        echo "⚠️ Skipping Visual Studio Subscription: $subscription_display_name at $subscription"
        continue
    fi

    #uncomment below if want to filter further for cis (in terms of entire subscriptions): 7.7
    # postgres_servers_in_subscription=$(az postgres server list --subscription="$subscription" --output="json" 2>/dev/null)
    # if [[ "$postgres_servers_in_subscription" == "[]" ]]; then
    #     echo "⚠️ No Postgres servers found in entire subscription: $subscription"
    #     continue
    # fi

    #uncomment below if want to filter further for cis (in terms of entire subscriptions): 4.1.1
    # role_assignments_list=$(az role assignment list --subscription "$subscription" --all --output json 2>/dev/null)
    # if [[ $(echo "$role_assignments_list" | jq length) -eq 0 ]]; then
    #     echo "⚠️ No role assignments lists found in subscription: $subscription & resource group: $rg"
    #     continue
    # fi

    # Get resource groups in this subscription
    resource_groups=$(az group list --query "[].name" -o tsv)

    if [[ -z "$resource_groups" ]]; then
        echo "⚠️ No resource groups found in subscription: $subscription"
        continue
    fi

    # Loop through resource groups
    for rg in $resource_groups; do
        echo "▶️ Running test for $subscription / $rg"

        #uncomment below if want to filter further for cis: 2.6
        # azure_redis_list=$(az redis list --subscription="$subscription" --resource-group="$rg")
        # if [[ "$azure_redis_list" == "[]" ]]; then
        #     echo "⚠️ No azure redis lists found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis: 3.1
        # get_cosmosdb_list=$(az cosmosdb list --subscription="$subscription" --resource-group="$rg")
        # if [[ "$get_cosmosdb_list" == "[]" ]]; then
        #     echo "⚠️ No cosmosdb lists found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis (in terms of entire resource groups): 4.1.1
        # role_assignments_list=$(az role assignment list --subscription "$subscription" --all --output json 2>/dev/null)
        # if [[ $(echo "$role_assignments_list" | jq length) -eq 0 ]]; then
        #     echo "⚠️ No role assignments lists found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis (in terms of entire resource groups): 7.7
        # postgres_server_list=$(az postgres server list --subscription="$subscription" --resource-group="$rg" 2>/dev/null)
        # if [[ "$postgres_server_list" == "[]" ]]; then
        #     echo "⚠️ No postgres server list found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis: 8.1, 8.2, 8.3, 8.4
        # network_security_group_list=$(az network nsg list --subscription="$subscription" --resource-group="$rg" 2>/dev/null)
        # if [[ "$network_security_group_list" == "[]" ]]; then
        #     echo "⚠️ No network security groups found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis: 9.3.7
        # key_vault_group=$(az keyvault list --subscription="$subscription" --resource-group="$rg")
        # if [[ "$key_vault_group" == "[]" ]]; then
        #     echo "⚠️ No key vault groups found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis: 10.1, 10.2
        # sql_server_groups=$(az sql server list --subscription="$subscription" --resource-group="$rg")
        # if [[ "$sql_server_groups" == "[]" ]]; then
        #     echo "⚠️ No sql server groups found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        #uncomment below if want to filter further for cis: 10.3.2.1, 10.3.2.2, 10.3.2.3, 10.3.9
        # storage_account_list=$(az storage account list --subscription="$subscription" --resource-group="$rg" 2> /dev/null)
        # if [[ "$storage_account_list" == "[]" ]]; then
        #     echo "⚠️ No storage account list found in subscription: $subscription & resource group: $rg"
        #     continue
        # fi

        log_file="${log_dir}/csvOutput.csv"
        if [[ $p_CSV == "False" ]]; then
            log_file="${log_dir}/${subscription}_${rg}.log"
            $script_name --subscription "$subscription" --resource-group "$rg" > "$log_file" 2>&1 #can include --debug here but not recommended
        else
            if [[ ! -f "$log_file" ]]; then
                $script_name --subscription "$subscription" --resource-group "$rg" --csv > "$log_file" 2>&1
            else
                $script_name --subscription "$subscription" --resource-group "$rg" --csv | tail -n +2 >> "$log_file" 2>&1
            fi
        fi
        echo "✅ Completed: $subscription / $rg"
        echo "📝 Output logged to $log_file"
        echo "-------------------------------------------------------------------------"
    done
done

echo "🎉 All tests completed."
